#ifndef _MONITOR_LINK_LIST_H
#define _MONITOR_LINK_LIST_H

#include "monitor_h_4_all.h"

/**
 * 客户端信息结构体：
 * 封装了客户端socket套接字、
 * 接收线程id、发送线程id、
 * 客户端网络通信地址信息
 */
typedef struct client_info {
    int fd;
    pthread_t pid_rcv;
    pthread_t pid_snd;
    struct sockaddr_in addr;
} Client_info;

/**
 * 客户端链表结点：
 * 自增的结点号（后期可用于
 * 同一客户端收发线程通信的
 * 唯一key）、客户端信息结
 * 构体、单链表next指针
 */
typedef struct client_node {
    int client_no;
    struct client_info client_info;
    struct client_node * next;
} Client_node;

/**
 * 客户端链表：
 * 封装了链表this指针指向自己、
 * 链表头结点、链表结点个数、
 * 链表插入（头插）、链表删除、
 * 链表所有节点信息打印
 */
typedef struct client_list {
    struct client_list * this;
    Client_node * head;
    int size;
    int ( *insert )( struct client_list * list, void *data );
    int ( *del_by_no )( struct client_list * list, int no );
    int ( *del_by_fd )( struct client_list * list, int fd );
    int ( *del_all_node )( struct client_list * list );
    int ( *print_list )( struct client_list * list );
} Client_list;

/****************************************************
  Function:     insert
  Description:  链表插入函数，头插
  Input:        链表：
                    Client_list * list
                客户端信息结构体：
                    void * client_info
  Output:
  Return:       插入成功返回0，链表出错返回-1
  Others:
****************************************************/
int insert ( Client_list * list, void * client_info );

/****************************************************
  Function:     del_by_no
  Description:  通过客户端编号删除链表节点
  Input:        链表：
                    Client_list * list
                客户端编号：
                    int no
  Output:
  Return:       删除成功返回0，链表出错返回-1
  Others:
****************************************************/
int del_by_no ( Client_list * list, int no );

/****************************************************
  Function:     del_by_fd
  Description:  通过客户端套接字删除链表节点
  Input:        链表：
                    Client_list * list
                客户端套接字：
                    int fd
  Output:
  Return:       删除成功返回0，链表出错返回-1
  Others:
****************************************************/
int del_by_fd ( Client_list * list, int fd );

/****************************************************
  Function:     del_all_node
  Description:  删除链表所有节点，不包括头结点
  Input:        链表：
                    Client_list * list
  Output:
  Return:       删除成功返回0，链表出错返回-1
  Others:
****************************************************/
int del_all_node ( Client_list * list );

/****************************************************
  Function:     print_list
  Description:  打印链表信息
  Input:        链表：
                    Client_list * list
  Output:
  Return:
  Others:
****************************************************/
int print_list ( Client_list * list );

/****************************************************
  Function:     ListConstruction
  Description:  初始化客户端链表以及链表头结点
  Input:
  Output:
  Return:
  Others:       客户端链表
****************************************************/
Client_list * ListConstruction ();

#endif
