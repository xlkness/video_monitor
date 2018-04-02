#ifndef _MONITOR_SOCKET_WRAPPER_H
#define _MONITOR_SOCKET_WRAPPER_H

#include "monitor_h_4_all.h"
#include "monitor_socket_embedded.h"

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
int Socket_init ( Server_info * server );

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
int Socket_accept ( Server_info * server );

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
int Socket_close_by_fd ( Server_info * server, int fd );

/****************************************************
  Function:     Socket_send
  Description:  网络通信消息发送函数，若发送出现客户端
                连接失效情况，就清理此客户端资源，
  Input:        服务器信息结构体：
                    Server_info * server
                连接套接字：
                    int fd
  Output:
  Return:
  Others:
****************************************************/
int Socket_send ( Server_info * server, int fd,
    const unsigned char * send_buf, int buf_size );

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
    unsigned char * recv_buf, int buf_size );

#endif
