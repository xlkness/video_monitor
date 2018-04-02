#include "monitor_v4l2_uvc.h"


void get_cam_info(struct camera *cam)
{

	printf("[func]:%s,[line]:%d, 获取摄像头信息：\n", __func__, __LINE__);
	if(ioctl(cam->fd, VIDIOC_QUERYCAP, &cam->cap) < 0)
	{
		perror("query camera information error.\n");
		exit(0);
	}
	if(!(cam->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{
		printf("device is not a capture device.\n");
		exit(0);
	}
	printf("\nCapability Informations:\n");
    printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\nCapabilities: %d\n",
           cam->cap.driver,cam->cap.card,cam->cap.bus_info,
           (cam->cap.version>>16)&0XFF, (cam->cap.version>>8)&0XFF,cam->cap.version&0XFF,
           cam->cap.capabilities );
    printf("\n\n");
}
void get_cam_fmtdesc(struct camera *cam)
{
	printf("[func]:%s,[line]:%d,获取摄像头图像编码格式\n", __func__, __LINE__);
	cam->fmtdesc.index 	= 0;
	cam->fmtdesc.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;

	while ((ioctl(cam->fd, VIDIOC_ENUM_FMT, &cam->fmtdesc)) == 0)
    {
        printf("\t%d.\n\t{\n\tpixelformat = '%c%c%c%c',\n\tdescription = '%s'\n \t}\n",
                cam->fmtdesc.index+1,
                cam->fmtdesc.pixelformat & 0xFF,
               (cam->fmtdesc.pixelformat >> 8) & 0xFF,
               (cam->fmtdesc.pixelformat >> 16) & 0xFF,
               (cam->fmtdesc.pixelformat >> 24) & 0xFF,
                cam->fmtdesc.description);
        	    cam->fmtdesc.index++;
    }
    printf("\n\n");
}
void set_cap_format(struct camera *cam)
{
	printf("[func]:%s,[line]:%d,设置摄像头图像采集格式\n", __func__, __LINE__);
	memset(&cam->format, 0, sizeof(cam->format));

	cam->format.fmt.pix.width 		= cam->width;
	cam->format.fmt.pix.height 		= cam->height;
	cam->format.fmt.pix.pixelformat = cam->cap_format;
	cam->format.fmt.pix.priv		= 1;
	cam->format.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	//cam->format.fmt.pix.field       = V4L2_FIELD_ANY;
	cam->format.type				= V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(ioctl(cam->fd, VIDIOC_S_FMT, &cam->format) < 0)
	{
		perror("set capture format error.\n");
		exit(0);
	}
	printf("\n\n");
}
void get_cap_format(struct camera *cam)
{
	printf("[func]:%s,[line]:%d,获取摄像头采集图像格式\n", __func__, __LINE__);
	struct v4l2_format fmt;
	struct v4l2_fmtdesc fmtdesc;
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(cam->fd, VIDIOC_G_FMT, &fmt) < 0)
	{
		perror("get_cap_format:error.\n");
		exit(0);
	}
	printf("\nCurrent data format information:\n width:%d\n height:%d\n",
           fmt.fmt.pix.width,fmt.fmt.pix.height);

	fmtdesc.index 		= 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	while(ioctl(cam->fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)
	{
		if(fmtdesc.pixelformat == cam->format.fmt.pix.pixelformat)
		{
			printf(" format:%s\n",fmtdesc.description);
			break;
		}
		fmtdesc.index ++;
	}
	printf("\n\n");
}
void get_framerate(struct camera *cam)
{
	printf("[func]:%s,[line]:%d,获取摄像头帧率\n", __func__, __LINE__);
	int ret;
	memset(&cam->setfps, 0, sizeof(struct v4l2_streamparm));
	cam->setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;
	cam->setfps.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
	ret = ioctl(cam->fd, VIDIOC_G_PARM, &cam->setfps);
	if(ret < 0)
	{
		perror("get fps error.\n");
	}
	printf(" fps:%d\n",cam->setfps.parm.capture.timeperframe.denominator);
	printf("\n\n");
}
void set_framerate(struct camera *cam)
{
	printf("[func]:%s,[line]:%d,设置摄像头帧率\n", __func__, __LINE__);
	int ret ;
	memset(&cam->setfps, 0, sizeof(struct v4l2_streamparm));
	cam->setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	cam->setfps.parm.capture.timeperframe.numerator = 1;
	cam->setfps.parm.capture.timeperframe.denominator = cam->fps;
	ret = ioctl(cam->fd, VIDIOC_S_PARM, &cam->setfps);
	if(ret < 0)
	{
		perror("set fps error.\n");
	}
	printf("\n\n");
}
void req_buffer(struct camera *cam)
{
	memset(&cam->reqbuf, 0, sizeof(cam->reqbuf));

	cam->reqbuf.count	= cam->buf_num;
	cam->reqbuf.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	cam->reqbuf.memory	= V4L2_MEMORY_MMAP;

	if(ioctl(cam->fd, VIDIOC_REQBUFS, &cam->reqbuf) < 0)
	{
		perror("reqbuf error.\n");
		exit(0);
	}
}
void init_mmap(struct camera *cam)
{

	int i;
	for(i=0; i<cam->buf_num; i++)
	{
		memset(&cam->buf, 0, sizeof(cam->buf));

		cam->buf.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		cam->buf.memory	= V4L2_MEMORY_MMAP;
		cam->buf.index 	= i;

		if(-1 == ioctl(cam->fd, VIDIOC_QUERYBUF, &cam->buf))
		{
			perror("ioctl querybuf error.\n");
			exit(0);
		}

		cam->buffers[i].length = cam->buf.length;

		cam->buffers[i].start = (unsigned char *)mmap(NULL,
								cam->buf.length,
								PROT_READ | PROT_WRITE,
								MAP_SHARED,
								cam->fd,
								cam->buf.m.offset);

		if(MAP_FAILED == cam->buffers[i].start)
		{
			perror("mmap error.\n");
			exit(0);
		}
	}
}
void start_capture(struct camera *cam)
{
	int i;

	for(i=0; i<cam->buf_num; i++)
	{
		//memset(&cam->buf, 0, sizeof(cam->buf));
		cam->buf.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		cam->buf.memory		= V4L2_MEMORY_MMAP;
		cam->buf.index		= i;

		if(-1 == ioctl(cam->fd, VIDIOC_QBUF, &cam->buf))
		{
			perror("start capture:video ioctl qbuf error.\n");
			exit(0);
		}
	}
	if(-1 == ioctl(cam->fd, VIDIOC_STREAMON, &cam->buf.type))
	{
		perror("start capture:video ioctl streamon error.\n");
		exit(0);
	}
}
int isHaveData(int fd)
{
	fd_set fds;
	struct timeval tv;
	int r;

	FD_ZERO (&fds);//将指定的文件描述符集清空
	FD_SET (fd, &fds);//在文件描述符集合中增加一个新的文件描述符

	tv.tv_sec = 2;
	tv.tv_usec = 0;
	for (;;) //这一段涉及到异步IO
	{
		r = select (fd + 1, &fds, NULL, NULL, &tv);//判断是否可读（即摄像头是否准备好），tv是定时
		if (-1 == r)
		{
			if (EINTR == errno)
			{
				printf("select err\n");
				continue;
			}
		}
		if (0 == r)
		{
			fprintf (stderr, "select timeout：%d\n",errno);
			return 0;
		}
		return 1;
	}
}
int get_one_frame_buf(struct camera * cam)
{
	int index ;
	int ret = 0;

	if(isHaveData(cam->fd))
	{
		memset(&cam->buf, 0, sizeof(cam->buf));

		cam->buf.type 		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		cam->buf.memory 	= V4L2_MEMORY_MMAP;

		ret = ioctl(cam->fd, VIDIOC_DQBUF, &cam->buf);
		if( ret < 0)
		{
			printf("[func]:%s,[line]:%d,ret:%d\n", __func__, __LINE__, ret);
			perror("get one frame buffer:video ioctl dqbuf error.\n");
			exit(0);
		}

		//assert(cam->buf.index < cam->buf_num);

		//memset(&cam->buf, 0, sizeof(cam->buf));
		index = cam->buf.index;

		//starter = (unsigned char *)buffers[cam->buf.index].start;
		ret = ioctl(cam->fd, VIDIOC_QBUF, &cam->buf);
		if( ret < 0)
		{
			printf("[func]:%s,[line]:%d,errno:%d,EGAING:%d,EINVAL:%d,EIO:%d,EPIPE:%d\n",
				__func__, __LINE__, errno, EINVAL, EINVAL, EIO, EPIPE);
			perror("get one frame buffer:video ioctl qbuf error.\n");
			exit(0);
		}
		return index;
	}
	else
	{
		printf("get one frame buffer error.\n");
		exit(0);
	}
}
void stop_capture(struct camera *cam)
{
	if(-1 == ioctl(cam->fd,VIDIOC_STREAMOFF,&cam->fmtdesc.type))
	{
		perror("stop capture error.\n");
		exit(0);
	}

	close(cam->fd);

	printf("stop capture successfully!\n");
}
void Munmap(int req_num, struct mmap_buffer *buffers)
{
	int i;
	for (i = 0; i < req_num; ++i)
   		if (-1 == munmap(buffers[i].start, buffers[i].length))
       		free (buffers);
}
void Init_v4l2(struct camera *cam)
{
	get_cam_info(cam);
	get_cam_fmtdesc(cam);
	set_cap_format(cam);
	get_cap_format(cam);
	set_framerate(cam);
	get_framerate(cam);
	req_buffer(cam);
	init_mmap(cam);
}
