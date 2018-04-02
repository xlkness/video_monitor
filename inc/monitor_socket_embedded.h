#ifndef _MONITOR_SOCKET_H
#define _MONITOR_SOCKET_H

#include "monitor_h_4_all.h"
#include "monitor_v4l2_uvc.h"
#include "monitor_link_list.h"

typedef struct server_info {
    int fd;
    unsigned int port;
    int domain;
    int type;
    int recv_timeout;
    int send_timeout;
    int listen_num;
    struct sockaddr_in  * addr;
    struct client_list  * client_list;
    struct camera         cam;
} Server_info;

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
    int protocol,int rTimeout, int sTimeout );

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
int socket_keeplive ( int socket_fd );

/****************************************************
  Function:     socket_bind
  Description:  绑定套接字和服务器信息
  Input:        监听套接字：
                    int socket_fd

  Output:
  Return:       正确返回套接字，错误返回-1
  Others:
****************************************************/
int socket_bind ( int socket_fd, unsigned int port );

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
int socket_listen ( int socket_fd, int listen_num );

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
int socket_accept ( int socket_fd, struct sockaddr_in * c_addr );

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
    int buf_size, int flag );

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
    int buf_size, int flag );

/****************************************************
  Function:     socket_fflush
  Description:  清空通信接收缓冲区
  Input:        连接套接字：
                    int socket_fd
  Output:
  Return:       正确返回0，未做错误判断
  Others:       一般用在准备接收消息前
****************************************************/
int socket_fflush ( int socket_fd );

/****************************************************
  Function:     socket_isConnect
  Description:  判断连接是否保持
  Input:        连接套接字：
                    int socket_fd
  Output:
  Return:       连接保持返回1，断开返回0
  Others:
****************************************************/
int socket_isConnect ( int socket_fd );

/****************************************************
  Function:     socket_close
  Description:  关闭连接
  Input:        连接套接字：
                    int socket_fd
  Output:
  Return:       连接保持返回1，断开返回0
  Others:       先关闭连接的读写功能，再关闭连接
****************************************************/
int socket_close ( int socket_fd );

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
int socket_init ( unsigned int port, int listen_num, int rTimeout, int sTimeout );

#endif
