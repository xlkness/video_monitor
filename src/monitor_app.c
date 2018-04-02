#include "monitor_h_4_all.h"
#include "monitor_link_list.h"
#include "monitor_socket_embedded.h"
#include "monitor_socket_wrapper.h"
#include "monitor_utils.h"
#include "monitor_v4l2_uvc.h"
#include "monitor_main_opt.h"

int main( int argc, char * argv[] )
{
    int ret = 0;
    Server_info server;

    server.port           = 8888;
    server.listen_num     = 5;
    server.recv_timeout   = 5;
    server.send_timeout   = 5;

    server.cam.fd         = open( "/dev/video2", O_RDWR | O_NONBLOCK, 0);
    server.cam.fps        = 25;
    server.cam.width      = 640;
    server.cam.height     = 480;
    server.cam.cap_format = V4L2_PIX_FMT_MJPEG;
    server.cam.buf_num    = 4;

    ret = Socket_init( &server );
    if ( ret < 0 ) {
        printf("[func]:%s, [line]:%d, 初始化socket监听套接字失败！！退出程序！\n", __func__, __LINE__);
        return 0;
    }
    printf("[func]:%s, [line]:%d, 初始化socket监听套接字成功！！！\n", __func__, __LINE__);

    Init_v4l2( &server.cam );
    printf("[func]:%s, [line]:%d, 初始化摄像头成功！！！\n", __func__, __LINE__);

    start_capture( &server.cam );

    main_opt( &server );

    return 0;
}
