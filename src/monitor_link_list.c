
#include "monitor_link_list.h"

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
int insert ( Client_list * list, void * client_info )
{
    if ( NULL == list ) {
        return -1;
    }

    Client_node * node = ( Client_node * )client_info;

    node->client_no = list->head->client_no + 1;
    node->next = list->head->next;
    list->head->next = node;
    list->head->client_no ++;
    list->size ++;

    return 0;
}
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
int del_by_no ( Client_list * list, int no )
{
    if ( NULL == list || NULL == list->head ) {
        return -1;
    }

    Client_node * p, * q;

    p = list->head->next;
    q = list->head;

    while ( p != NULL ) {
        if ( p->client_no == no ) {
            break;
        }
        q = p;
        p = p->next;
    }

    if( NULL == p ) {
        return -1;
    }

    q->next = p->next;
    free(p);
    list->size --;

    return 0;
}
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
int del_by_fd ( Client_list * list, int fd )
{
    if ( NULL == list || NULL == list->head ) {
        return -1;
    }

    Client_node * p, * q;

    p = list->head->next;
    q = list->head;

    while ( p != NULL ) {
        if ( p->client_info.fd == fd ) {
            break;
        }
        q = p;
        p = p->next;
    }

    if ( NULL == p ) {
        return -1;
    }

    q->next = p->next;
    free(p);
    list->size --;

    return 0;
}
/****************************************************
  Function:     del_all_node
  Description:  删除链表所有节点，不包括头结点
  Input:        链表：
                    Client_list * list
  Output:
  Return:       删除成功返回0，链表出错返回-1
  Others:
****************************************************/
int del_all_node ( Client_list * list )
{
    if ( NULL == list || NULL == list->head ) {
        return -1;
    }
    if ( NULL == list->head->next ) {
        return 0;
    }

    Client_node * p;

    p = list->head;

    while ( list->head->next != NULL ) {
        p = list->head->next;
        list->head->next = p->next;
        free(p);
    }

    return 0;
}
/****************************************************
  Function:     print_list
  Description:  打印链表信息
  Input:        链表：
                    Client_list * list
  Output:
  Return:
  Others:
****************************************************/
int print_list ( Client_list * list )
{
    if ( NULL == list ) {
        printf( "[func]:%s,[line]:%d, list is empty!\n", __func__, __LINE__ );
        return -1;
    }

    int i = 0;
    char * ip = NULL;
    Client_node * p = list->head;

    printf( "\n[func]:%s,[line]:%d\n------------------------------------------------\n", __func__, __LINE__ );
    for ( i = 0; i < list->size; i ++ ) {
        p = p->next;
        ip = inet_ntoa( ( struct in_addr )p->client_info.addr.sin_addr );
        printf( "client :%d, fd :%d, ip address is :%s\n", p->client_no,
            p->client_info.fd, ip );
    }
    printf("------------------------------------------------\n\n");

    return ;
}
/****************************************************
  Function:     ListConstruction
  Description:  初始化客户端链表以及链表头结点
  Input:
  Output:
  Return:       客户端链表
  Others:
****************************************************/
Client_list * ListConstruction ()
{
    Client_node * head = NULL;
    Client_list * list = NULL;

    head = ( Client_node * ) malloc (
                sizeof ( Client_node ) );
    list = ( Client_list * ) malloc (
                sizeof ( Client_list ) );

    head->client_no = 0;
    head->next = NULL;

    list->head = head;

    list->size = 0;
    list->insert = insert;
    list->del_by_no = del_by_no;
    list->del_by_fd = del_by_fd;
    list->del_all_node = del_all_node;
    list->print_list = print_list;

    list->this = list;

    return ( Client_list * ) list;
}
#if 0

int main()
{
    Client_list * list = ListConstruction();

    Client_info * node1 = ( Client_info * ) malloc (
                                sizeof ( Client_info ) );
    Client_info * node2 = ( Client_info * ) malloc (
                                sizeof ( Client_info ) );

    node1->fd = 1;
    node2->fd = 2;

    list->insert( list->this, ( void * ) node1 );
    list->insert( list->this, ( void * ) node2 );

    list->print_list( list->this );
}

#endif
