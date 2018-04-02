
#include "monitor_socket_wrapper.h"

/****************************************************
  Function:     Socket_init
  Description:  服务器初始化， 包括监听套接字创建、
                客户端单链表初始化
  Input:        服务器信息结构体：
                    Server_info * server
  Output:
  Return:       初始化成功返回0，链表出错返回-1
  Others:
****************************************************/
int Socket_init ( Server_info * server )
{
    int socket_fd = 0;

    server->fd = socket_init( server->port, server->listen_num,
        server->recv_timeout, server->send_timeout );
    if ( server->fd < 0 ) {
        printf( "[func]:%s,[line]:%d,监听套接字初始化失败！\n", __func__, __LINE__ );
        return -1;
    }

    server->client_list = ListConstruction();

    return 0;
}
/****************************************************
  Function:     Socket_accept
  Description:  连接套接字创建，即监听客户端连接，有
                连接就初始化一个客户端结点，插入客户
                端链表
  Input:        服务器信息结构体：
                    Server_info * server
  Output:
  Return:       初始化成功返回0，链表出错返回-1
  Others:
****************************************************/
int Socket_accept ( Server_info * server )
{
    if ( NULL == server->client_list || NULL == server->client_list->head ) {
        printf("[func]:%s, [line]:%d, 客户端链表或链表头为空\n", __func__, __LINE__);
        return -1;
    }

    int ret = 0;
    Client_node * client_node = NULL;

    client_node = ( Client_node * )malloc( sizeof( Client_node ) );

    ret = socket_accept( server->fd, &client_node->client_info.addr );
    if ( ret < 0 ) {
        printf("[func]:%s, [line]:%d, 底层accept返回失败！！\n", __func__, __LINE__);
        return -1;
    }
    printf("[func]:%s, [line]:%d, accept成功，套接字：%d，增加一个客户端成功！！\n", __func__, __LINE__, ret);
    client_node->client_info.fd = ret;

    server->client_list->insert( server->client_list->this, client_node );

    server->client_list->print_list( server->client_list->this );

    return 0;
}
/****************************************************
  Function:     Socket_close_by_fd
  Description:  根据连接套接字删除客户端链表的客户端
                结点
  Input:        服务器信息结构体：
                    Server_info * server
                连接套接字：
                    int fd
  Output:
  Return:
  Others:
****************************************************/
int Socket_close_by_fd ( Server_info * server, int fd )
{
    server->client_list->del_by_fd( server->client_list->this, fd );
    socket_close( fd );
}
/****************************************************
  Function:     Socket_send
  Description:  网络通信消息发送函数，若发送出现客户端
                连接失效情况，就清理此客户端资源，
  Input:        服务器信息结构体：
                    Server_info * server
                连接套接字：
                    int fd
                发送缓冲区：
                    const unsigned char * send_buf
                缓冲区大小：
                    buf_size
  Output:
  Return:
  Others:
****************************************************/
int Socket_send ( Server_info * server, int fd,
    const unsigned char * send_buf, int buf_size )
{
    if ( NULL == server->client_list || NULL == server->client_list->head ) {
        printf("[func]:%s, [line]:%d, 客户端链表或链表头为空\n", __func__, __LINE__);
        return -1;
    }
    if ( NULL == send_buf) {
        return -1;
    }

    int ret = 0;

    ret = socket_send ( fd, send_buf, buf_size, 0 );
    if ( ret < 0 ) {
        Socket_close_by_fd( server, fd );
        printf("[func]:%s, [line]:%d, [发送]--套接字为%d的客户端断开通信或其它错误，回收其资源，此时所有客户端信息：\n",
            __func__, __LINE__, fd);
        return -1;
    }

    return 0;
}
/****************************************************
  Function:     Socket_recv
  Description:  网络通信消息接收函数，若接收出现客户端
                连接失效情况，就清理此客户端资源，
  Input:        服务器信息结构体：
                    Server_info * server
                连接套接字：
                    int fd
                接收缓冲区：
                    unsigned char * recv_buf
                缓冲区大小：
                    buf_size
  Output:
  Return:
  Others:
****************************************************/
int Socket_recv ( Server_info * server, int fd,
    unsigned char * recv_buf, int buf_size )
{
    if ( NULL == server->client_list || NULL == server->client_list->head ) {
        printf("[func]:%s, [line]:%d, 客户端链表或链表头为空\n", __func__, __LINE__);
        return -1;
    }

    int ret = 0;

    ret = socket_recv ( fd, recv_buf, buf_size, 0 );
    if ( ret < 0 ) {
        Socket_close_by_fd( server, fd );
        printf("[func]:%s, [line]:%d, [接收]--套接字为%d的客户端断开通信或其它错误，回收其资源，此时所有客户端信息：\n",
            __func__, __LINE__, fd);
        return -1;
    }

    return 0;
}

