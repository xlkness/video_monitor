#include "monitor_main_opt.h"

static int rec_flag = 0;

void * send_handler( void * data )
{
    Server_info * server = ( Server_info * )data;

    int fd = server->client_list->head->next->client_info.fd;

    int msgkey = 1024;
    int msqid;

    int fd_jpg;
    int ret = 0;
    int cam_buf_index = -1;
    int jpg_len = 0;

    FILE * fp_avi = NULL;

    unsigned char jpg_file_name[20] = {0};
    unsigned char avi_file_name[20] = {0};
    unsigned char send_len[10] = {0};
    unsigned char send_buf[1024*100] = {0};

    time_t tt;
    struct timeval tmp_tt1;
    struct timeval tmp_tt2;
    int timeinterval;

    struct tm * tm = NULL;
    struct msg_buf msg_buf;

    msqid = msq_init( msgkey );
    if ( msqid < 0 ) {
        printf("[func]:%s,[line]:%d,初始化消息队列失败！！\n", __func__, __LINE__);
        exit( 0 );
    }

    while ( 1 ) {

        memset( &msg_buf, 0, sizeof(struct msg_buf) );
        memset( send_len, 0, sizeof( send_len ) ) ;

        cam_buf_index = get_one_frame_buf( &server->cam );

        jpg_len = create_jpg( send_buf, ( unsigned char * )server->cam.buffers[cam_buf_index].start,
                                server->cam.buffers[cam_buf_index].length);

        ret = msq_recv_noblock( msqid, &msg_buf, sizeof(struct msg_buf) - sizeof(long), msgkey );
        if ( ret > 0) {
            switch( msg_buf.opt ) {
                case CAM_CAP:
                    tt = time( NULL );
                    tm = localtime( &tt );
                    memset( jpg_file_name, 0, sizeof( jpg_file_name ) );
                    sprintf(jpg_file_name, "/sdcard/capture/%d%.2d%.2d%.2d%.2d%.2d.jpg",
                        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

                    fd_jpg = open( jpg_file_name, O_RDWR | O_CREAT, 0666);
                    if ( fd_jpg < 0 ) {
                        printf("[func]:%s, [line]:%d, 响应拍照命令失败，原因：创建照片失败！\n", __func__, __LINE__);
                    } else {
                        ret = write( fd_jpg, send_buf, jpg_len );
                        if ( ret < 0 ) {
                            printf("[func]:%s, [line]:%d, 响应拍照命令失败，原因：写入照片照片文件失败！\n", __func__, __LINE__);
                        } else {
                            printf("[func]:%s, [line]:%d, 响应拍照命令成功！\n", __func__, __LINE__);
                        }
                        close( fd_jpg );
                    }
                    break;
                case CAM_REC:
                    tt = time( NULL );
                    tm = localtime( &tt );
                    sprintf(avi_file_name, "/sdcard/record/%d%.2d%.2d%.2d%.2d%.2d.avi",
                        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

                    fp_avi = fopen( avi_file_name, "wb+" );
                    if ( NULL == fp_avi ) {
                        printf("[func]:%s, [line]:%d, 响应录像命令失败，原因：创建录像文件失败！\n", __func__, __LINE__);
                        rec_flag = 0;
                        break;
                    }

                    if ( 0 == rec_flag )
                        create_avi( fp_avi, server->cam.fps - 10, server->cam.width, server->cam.height );

                    rec_flag = 1;
                    break;
                case CAM_STOP_REC:
                    rec_flag = 0;
                    write_tail( fp_avi, server->cam.fps - 10, server->cam.width, server->cam.height );
                    fclose( fp_avi );
                    printf("[func]:%s, [line]:%d, 保存录像文件成功！\n", __func__, __LINE__);
                    break;
                case CLIENT_CLOSE:
                    if ( 1 == rec_flag ) {
                        write_tail( fp_avi, server->cam.fps - 10, server->cam.width, server->cam.height );
                        fclose( fp_avi );
                        printf("[func]:%s, [line]:%d, 保存录像文件成功！\n", __func__, __LINE__);
                    }
                    rec_flag = 0;
                    break;
                default:

                    break;
            }
        }

        sprintf( send_len, "%d", jpg_len );

        if( 1 == rec_flag ) {
            //gettimeofday( &tmp_tt1, NULL );
            //printf("[func],%s,[line]:%d,准备封装一帧jpg-------。。。\n", __func__, __LINE__);
            add_jpg_to_avi( send_buf, fp_avi, jpg_len );
            //gettimeofday( &tmp_tt2, NULL );
            //timeinterval = 1000000 * ( tmp_tt2.tv_sec - tmp_tt1.tv_sec ) + tmp_tt2.tv_usec - tmp_tt1.tv_usec;
            //printf("[func],%s,[line]:%d,封装一帧jpg成功！！-------时差:%d\n",
            //    __func__, __LINE__, timeinterval) ;
        }
        ret = Socket_send( server, fd, send_len, strlen( send_len ) ) ;
        if ( ret < 0 ) {
            break;
        }

        ret = Socket_send( server, fd, send_buf, jpg_len );
        if ( ret < 0 ) {
            break;
        }

        usleep( 5 );
    }
}
void * recv_handler( void * data )
{
    Server_info * server = ( Server_info * )data;

    int fd = server->client_list->head->next->client_info.fd;
    int ret = 0;
    int msgkey = 1024;
    int msqid;

    unsigned char recv_buf[1024] = {0};
    struct msg_buf msg_buf;

    msqid = msq_init( msgkey );
    if ( msqid < 0 ) {
        printf("[func]:%s,[line]:%d,初始化消息队列失败！！\n", __func__, __LINE__);
        exit( 0 );
    }

    while ( 1 ) {
        memset( &msg_buf, 0, sizeof(msg_buf) );
        memset( recv_buf, 0, sizeof( recv_buf ) );

        ret = Socket_recv( server, fd, recv_buf, sizeof( recv_buf ) );
        if ( ret >= 0 ) {
            printf("[func]:%s, [line]:%d, 接收到消息为:%s\n", __func__, __LINE__, recv_buf);
            if ( !strcmp( recv_buf, "capture" ) ) {
                msg_buf.type = msgkey;
                msg_buf.opt = CAM_CAP;
                ret = msq_send( msqid, &msg_buf, sizeof(struct msg_buf) - sizeof(long));
                if ( ret < 0 ) {
                    printf("[func]:%s, [line]:%d, 通知拍照线程拍照失败！\n", __func__, __LINE__);
                }
            }
            else if ( !strcmp( recv_buf, "record" ) ) {
                msg_buf.type = msgkey;
                msg_buf.opt = CAM_REC;
                ret = msq_send( msqid, &msg_buf, sizeof(struct msg_buf) - sizeof(long));
                if ( ret < 0 ) {
                    printf("[func]:%s, [line]:%d, 通知拍照线程录像失败！\n", __func__, __LINE__);
                }
            }
            else if ( !strcmp( recv_buf, "stoprecord" ) ) {
                msg_buf.type = msgkey;
                msg_buf.opt = CAM_STOP_REC;
                ret = msq_send( msqid, &msg_buf, sizeof(struct msg_buf) - sizeof(long));
                if ( ret < 0 ) {
                    printf("[func]:%s, [line]:%d, 通知拍照线程停止录像失败！\n", __func__, __LINE__);
                }
            }
            else if ( !strcmp( recv_buf, "exit") ) {
                msg_buf.type = msgkey;
                msg_buf.opt = CLIENT_CLOSE;
                ret = msq_send( msqid, &msg_buf, sizeof(struct msg_buf) - sizeof(long));
                if ( ret < 0 ) {
                    printf("[func]:%s, [line]:%d, 通知拍照线程停止录像失败！\n", __func__, __LINE__);
                }
            }

            usleep( 50000 );
            continue;
        }
        break;
    }
}

int main_opt( Server_info * server )
{
    int ret = 0;
    pthread_attr_t  attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //send_handler( server );
    while ( 1 ) {
        ret = Socket_accept( server );
        if ( 0 == ret ) {
            pthread_create( &(server->client_list->head->next->client_info.pid_rcv),
                &attr, recv_handler, server );
            pthread_create( &(server->client_list->head->next->client_info.pid_snd),
                &attr, send_handler, server );
        }
    }

    return 0;
}

