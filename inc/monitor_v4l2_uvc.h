#ifndef _V4L2_UVC_H
#define _V4L2_UVC_H
#include "monitor_utils.h"
#include "monitor_h_4_all.h"
struct mmap_buffer{
	void *start;
	size_t length;
};
typedef struct camera{
	int fd;
	int fps;
	int width;
	int height;
	int cap_format;
	int buf_num;
	struct v4l2_capability 		cap;
	struct v4l2_fmtdesc 		fmtdesc;
	struct v4l2_format 			format;
	struct v4l2_streamparm 		setfps;
	struct v4l2_requestbuffers 	reqbuf;
	struct v4l2_buffer 			buf;
	struct mmap_buffer 			buffers[4];
}Camera;

void get_cam_info(struct camera *cam);

void get_cam_fmtdesc(struct camera *cam);

void set_cap_format(struct camera *cam);

void get_cap_format(struct camera *cam);

void get_framerate(struct camera *cam);

void set_framerate(struct camera *cam);

void req_buffer(struct camera *cam);

void init_mmap(struct camera *cam);

void start_capture(struct camera *cam);

int get_one_frame_buf(struct camera * cam);

void stop_capture(struct camera *cam);

void Munmap(int req_num, struct mmap_buffer *buffers);

void Init_v4l2(struct camera *cam);

#endif
