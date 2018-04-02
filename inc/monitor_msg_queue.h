#ifndef _MONITOR_MSG_QUEUE_H
#define _MONITOR_MSG_QUEUE_H

#include "monitor_h_4_all.h"

enum opt {
        CAM_CAP = 1,
        CAM_REC,
        CAM_STOP_REC,
        CLIENT_CLOSE
};

struct msg_buf {
    long type;
    enum opt opt;
};

/*************************************************
  Function:     msq_init
  Description:  消息队列初始化
  Input:        消息队列唯一通道key：
                    int key
  Output:
  Return:       成功返回消息队列id，失败返回-1
  Others:
*************************************************/
int msq_init(int key);

/*************************************************
  Function:     msq_send
  Description:  消息队列发送，判断发送失败会重发，
                重发次数为count数
  Input:        消息队列id：
                    int msqid
                发送的消息队列缓冲区：
                    void *buf
                发送的消息长度
                    int len
  Output:
  Return:       成功返回0，失败返回-1
  Others:
*************************************************/
int msq_send(int msqid, void *buf, int len);

/*************************************************
  Function:     msq_recv_timeout
  Description:  封装的消息等待队列的接收操作，接收
                方式为非阻塞，且根据输入参数设置的
                超时时间来作为循环检测消息队列次数，
                次数=sec * 1000 / 500
  Input:        消息队列id：
                    int msqid
                消息队列消息类型：
                    int mstype
                接收超时时间：
                    int sec_wait
                接收消息长度：
                    int len
  Output:       用于接收消息的缓冲区
                    void *buf
  Return:       成功返回接收字节数，失败返回-1
  Others:
*************************************************/
int msq_recv_timeout(int msqid, void *buf, int len,
    int mstype, int sec_wait);

/*************************************************
  Function:     msq_recv_noblock
  Description:  封装的消息等待队列的接收操作，接收
                方式为非阻塞，立即检测一次消息队列
                有无消息
  Input:        消息队列id：
                    int msqid
                消息队列消息类型
                    int mstype
                接收消息长度：
                    int len
  Output:       用于接收消息的缓冲区
                    void *buf
  Return:       成功返回接收字节数，失败返回-1
  Others:
*************************************************/
int msq_recv_noblock(int msqid, void *buf, int len, int mstype);

/*************************************************
  Function:     msq_recv_block
  Description:  封装的消息等待队列的接收操作，接收
                方式为阻塞
  Input:        消息队列id：
                    int msqid
                消息队列消息类型
                    int mstype
                接收消息长度：
                    int len
  Output:       用于接收消息的缓冲区
                    void *buf
  Return:       成功返回接收字节数，失败返回-1
  Others:
*************************************************/
int msq_recv_block(int msqid, void *buf, int len, int mstype);

/*************************************************
  Function:     msq_close
  Description:  封装的消息等待队列的关闭操作
  Input:        消息队列id：
                    int msqid
  Output:
  Return:       成功返回0，失败返回-1
  Others:
*************************************************/
int msq_close(int msqid);

#endif
