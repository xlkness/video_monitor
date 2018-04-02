
#include "monitor_socket_embedded.h"

#undef DEBUG_SOCKET

/****************************************************
  Function:     socket_create
  Description:  服务器套接字创建函数，包括在初始化套
                接字、设置链路收发超时时长
  Input:        网络层协议簇(AF_INET/AF_INET6)：
                    int domain
                套接口类型(SOCK_STREAM/SOCK_DGRAM)：
                    int type
                传输层传输协议(IPPROTO_TCP/IPPROTO_UPD/IPPROTO_SCTP):
                    int protocol
                接收超时时间(s)：
                    int rTimeOut
                发送超时时间(s):
                    int sTimeOut
  Output:
  Return:       正确返回套接字，错误返回-1
  Others:
****************************************************/
int socket_create ( int domain, int type,
    int protocol,int rTimeout, int sTimeout )
{
    int socket_fd = -1;
    struct timeval tv;

    socket_fd = socket(domain, type, protocol);
    if( -1 == socket_fd ) {
        return -1;
    }

    tv.tv_usec = 0;
    tv.tv_sec = sTimeout;

    if ( setsockopt( socket_fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof( tv ) ) < 0 ) {
        return -1;
    }

    tv.tv_sec = rTimeout;

    if ( setsockopt( socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof( tv ) ) < 0 ) {
        return -1;
    }

    return socket_fd;
}
/****************************************************
  Function:     socket_keeplive
  Description:  开启tcp保活机制，使用linux tcp的keep-
                alive加TCP_USER_TIMEOUT机制，解决对端
                异常断网、掉电的情况下，连接被长期挂起
  Input:        监听套接字：
                    int socket_fd
  Output:
  Return:       正确返回0,未做错误判断
  Others:       务必包含头文件<#include <netinet/tcp.h>
****************************************************/
int socket_keeplive ( int socket_fd )
{
    int sock_alive       = 1;       //开启keepalive属性.缺省值:0(关闭)
    int sock_idle        = 5;      //如果在idle秒内没有任何数据交互则进行探测.缺省值:7200(s)
    int sock_interval    = 5;      //探测时发探测包的时间间隔为interval秒.缺省值:75(s)
    int sock_count       = 1;       //探测重试的次数.全部超时则认定连接失效.缺省值:9(次)

    unsigned int timeout = 1000;   //值为数据包被发送后未接收到ACK确认的最大时长，以毫秒
                                    //为单位，例如设置为timeout时，代表如果发送出去的数据包
                                    //在timeout毫秒内未收到ACK确认，则下一次调用send或者recv，
                                    //函数会返回-1，errno设置为ETIMEOUT，代表connection timeout。

    //使用TCP Keep-alive加上TCP_USER_TIMEOUT机制，就可以完美解
    //决通信对端异常断网、掉电的情况下，连接被长期挂起的问题了。
    setsockopt(socket_fd, IPPROTO_TCP, 18, &timeout, sizeof(timeout));
    //setsockopt(socket_fd, IPPROTO_TCP, TCP_USER_TIMEOUT, &timeout, sizeof(timeout));
    setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE, (void*)&sock_alive, sizeof(sock_alive));
    setsockopt(socket_fd, SOL_TCP, TCP_KEEPIDLE, (void*)&sock_idle, sizeof(sock_idle));
    setsockopt(socket_fd, SOL_TCP, TCP_KEEPINTVL, (void*)&sock_interval, sizeof(sock_interval));
    setsockopt(socket_fd, SOL_TCP, TCP_KEEPCNT, (void*)&sock_count, sizeof(sock_count));

    return 0;
}
/****************************************************
  Function:     socket_bind
  Description:  绑定套接字和服务器信息
  Input:        监听套接字：
                    int socket_fd

  Output:
  Return:       正确返回套接字，错误返回-1
  Others:
****************************************************/
int socket_bind ( int socket_fd, unsigned int port )
{
    int reuse_opt = 1;
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons( port );

    if ( setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR,
        ( char * ) &reuse_opt, sizeof ( reuse_opt ) ) < 0) {
        return -1;
    }

    if ( -1 == bind( socket_fd, (struct sockaddr *) &server_addr, sizeof( server_addr ) ) ) {
        return -1;
    }

    return 0;
}
/****************************************************
  Function:     socket_listen
  Description:  让服务器socket套接字进入监听状态，成为
                监听套接字
  Input:        监听套接字：
                    int socket_fd
                服务器可以排队的最大连接数：
                    int listen_num
  Output:
  Return:       正确返回0,错误返回-1
  Others:       socket默认是主动类型，listen会将
                socket变为被动类型，等待客户的连接
****************************************************/
int socket_listen ( int socket_fd, int listen_num )
{
    return listen( socket_fd, listen_num ) < 0 ? -1 : 0;
}
/****************************************************
  Function:     socket_accept
  Description:  阻塞进程，直到有一个客户端连接建立，则
                立即返回一个可用的新的套接字，这个套接
                字是连接套接字
  Input:        监听套接字：
                    int socket_fd
  Output:       客户端地址结构体：
                    struct sockaddrr_in * c_addr
  Return:       正确返回套接字，错误返回-1
  Others:
****************************************************/
int socket_accept ( int socket_fd, struct sockaddr_in * c_addr )
{
    int client_fd;
    socklen_t addr_len = sizeof( struct sockaddr_in );

    client_fd = accept( socket_fd, ( struct sockaddr * ) c_addr, &addr_len );
    if( client_fd < 0 ) {
        return -1;
    }

    return client_fd;
}
/****************************************************
  Function:     socket_send
  Description:  消息发送函数，若有发送失败，则判断是否
                为连接发生不可恢复错误
  Input:        连接套接字：
                    int socket_fd
                发送缓冲区：
                    unsigned char * send_buf
                发送缓冲区大小：
                    int buf_size
                发送标记(默认0,详细设置man send)：
                    int flag
  Output:
  Return:       正确返回发送字节数，错误返回-1
  Others:       若有返回-1，调用层一定要判断，且-1
                表示连接出现不可恢复错误，需要断开
                此连接，清理此连接相关资源
****************************************************/
int socket_send ( int socket_fd, const unsigned char * send_buf,
    int buf_size, int flag )
{
    int snd_bytes = 0;
    int snd_total_bytes = 0;
    int snd_count = 3;

    while ( snd_count -- ) {
        snd_bytes = send( socket_fd, send_buf, buf_size, flag );
        if ( snd_bytes <= 0 ) {
            if ( EAGAIN == errno || EINTR == errno
              || EWOULDBLOCK == errno ) { //暂时发送失败，需要重复发送
                usleep( 50 );
                continue;
            }else {  //连接不正常，返回-1交由上层清理此套接字
                printf("[func]:%s,[line]:%d,连接不正常，发送失败，返回-1\n", __func__, __LINE__);
                return -1;
            }
        }
        snd_total_bytes += snd_bytes;
        if ( snd_total_bytes >= buf_size ) {
            break;
        }
    }
    if ( !snd_count ) {
        return -1;
    }
    return snd_total_bytes;
}
/****************************************************
  Function:     socket_recv
  Description:  消息接收函数，若有接收失败，则判断是否
                为连接发生不可恢复错误
  Input:        连接套接字：
                    int socket_fd
                接收缓冲区：
                    unsigned char * send_buf
                接收缓冲区大小：
                    int buf_size
                接收标记(默认0,详细设置man recv)：
                    int flag
  Output:
  Return:       正确返回接收字节数，错误返回-1
  Others:       若有返回-1，调用层一定要判断，且-1
                表示连接出现不可恢复错误，需要断开
                此连接，清理此连接相关资源
****************************************************/
int socket_recv ( int socket_fd, unsigned char * recv_buf,
    int buf_size, int flag )
{
    int ret = -1;

    socket_fflush( socket_fd );

    memset( recv_buf, 0, buf_size );

    while( 1 ) {
        ret = recv( socket_fd, recv_buf, buf_size, flag );
        //printf("[func]:%s,[line]:%d,recv ret:%d,errno:%d,EAGAIN:%d\n",
        //    __func__, __LINE__, ret, errno, EAGAIN);
        perror("recv error");
        if ( 0 == ret ) { //对方主动且正常的关闭，返回0交由上层清理此套接字
            return -1;
        }
        else if ( ret < 0 ) {
            if ( EAGAIN == errno || EWOULDBLOCK == errno
              || EINTR == errno ) { //连接正常，接收失败，需要重复接收
                usleep( 50000 );
                continue;
            }else {    //连接不正常，返回0交由上层清理此套接字
                printf("[func]:%s,[line]:%d,连接不正常，errno:%d,recv返回-1\n",
                  __func__, __LINE__, errno);
                return -1;
            }
        }
        else {
            break;
        }
        usleep( 50000 );
    }

    return ret;
}
/****************************************************
  Function:     socket_fflush
  Description:  清空通信接收缓冲区
  Input:        连接套接字：
                    int socket_fd
  Output:
  Return:       正确返回0，未做错误判断
  Others:       一般用在准备接收消息前
****************************************************/
int socket_fflush ( int socket_fd )
{
    struct timeval timeout;
    fd_set fds;
    char tmp[2] = {0};

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(socket_fd, &fds);

    while ( 1 ) {
        if ( 0 == select(socket_fd + 1, &fds, NULL, NULL, &timeout ) ) {
            break;
        }
        if ( recv( socket_fd, tmp, 1, 0) != 1 ) {
            break;
        }
    }
    return 0;
}
/****************************************************
  Function:     socket_isConnect
  Description:  判断连接是否保持
  Input:        连接套接字：
                    int socket_fd
  Output:
  Return:       连接保持返回1，断开返回0
  Others:
****************************************************/
int socket_isConnect ( int socket_fd )
{
    struct tcp_info info;
    int len = sizeof( info );

    if (socket_fd <= 0 ) {
        return 0;
    }

    memset( &info, 0, sizeof(info) );

    //IPPROTO_TCP:IPv4套接口
    //必须包含 <netinet/tcp.h>
    getsockopt( socket_fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len );

    if ( TCP_ESTABLISHED == info.tcpi_state ) {
        return 1;
    }

    return 0;
}
/****************************************************
  Function:     socket_close
  Description:  关闭连接
  Input:        连接套接字：
                    int socket_fd
  Output:
  Return:       连接保持返回1，断开返回0
  Others:       先关闭连接的读写功能，再关闭连接
****************************************************/
int socket_close ( int socket_fd )
{
    shutdown( socket_fd, SHUT_RDWR );

    return close( socket_fd ) < 0 ? -1 : 0;
}
/****************************************************
  Function:     socket_init
  Description:  初始化监听套接字，包括创建套接字、开启
                保活机制、绑定、设置监听
  Input:        端口号：
                    unsigned int port
                最大监听连接数：
                    int listen_num
                接收超时时间：
                    int rTimeout
                发送超时时间：
                    int sTimeout
  Output:
  Return:       初始化成功返回监听套接字，错误返回-1
  Others:
****************************************************/
int socket_init ( unsigned int port, int listen_num, int rTimeout, int sTimeout )
{
    int socket_fd;
    int ret = -1;

    socket_fd = socket_create( AF_INET, SOCK_STREAM, 0, rTimeout, sTimeout );
    if ( socket_fd < 0 ) {
        return -1;
    }
    printf( "[func]:%s, [line]:%d, 创建套接字成功！\n", __func__, __LINE__ );

    ret = socket_keeplive( socket_fd );

    ret = socket_bind( socket_fd, port );
    if ( ret < 0 ) {
        return -1;
    }
    printf( "[func]:%s, [line]:%d, 绑定套接字和服务器信息成功！\n", __func__, __LINE__ );

    ret = socket_listen( socket_fd, listen_num );
    if ( ret < 0 ) {
        return -1;
    }
    printf( "[func]:%s, [line]:%d, 设置socket套接字监听成功！\n", __func__, __LINE__ );

    return socket_fd;
}
#ifdef DEBUG_SOCKET

int main()
{
    int ret = 0;
    int socket_fd;
    int client_fd;
    struct sockaddr_in client_addr;

    unsigned char buf[1024] = {0};

    socket_fd = socket_init( 8888, 5, 5, 5 );
    if ( socket_fd < 0 ) {
        printf(" [func]:%s, [line]:%d, 初始化监听套接字失败！！退出程序。\n", __func__, __LINE__ );
        return -1;
    }
    printf( "[func]:%s, [line]:%d, 初始化监听套接字成功！下面接收客户端连接！\n", __func__, __LINE__ );

    client_fd = socket_accept( socket_fd, &client_addr );
    if ( client_fd < 0 ) {
        printf( "[func]:%s, [line]:%d, 监听客户端连接失败！退出程序！\n", __func__, __LINE__ );
        return -1;
    }
    printf( "[func]:%s, [line]:%d, 监听客户端连接成功！\n", __func__, __LINE__ );

    int i = 0;

    // while ( 1 ) {
    //     ret = socket_send( client_fd, buf, sizeof( buf ), 0 );
    //     if ( ret < 0 ) {
    //         printf( "[func]:%s, [line]:%d, 接收出错，退出程序，errmsg:\n", __func__, __LINE__ );
    //         perror( "\n" );
    //         return -1;
    //     }
    //     printf( "[func]:%s, [line]:%d, 接收信息:%s\n", __func__, __LINE__, buf);
    //     memset( buf, 0, sizeof( buf ) );
    // }

    while ( 1 ) {
        sprintf( buf, "server send msg %d", i);
        ret = socket_send( client_fd, buf, strlen( buf ), 0 );
        if ( ret < 0 ) {
            printf( "[func]:%s, [line]:%d, 发送出错，退出程序，errmsg:\n", __func__, __LINE__ );
            perror( "\n" );
            return -1;
        }
        i ++;
        sleep( 2 );
    }

    return 0;
}

#endif
