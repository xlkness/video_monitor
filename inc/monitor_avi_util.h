#ifndef _MONITOR_AVI_UTIL_H
#define _MONITOR_AVI_UTIL_H

#include "monitor_h_4_all.h"

#define SWAP2(x) (((x>>8) & 0x00ff) | ((x<<8) & 0xff00))
#define SWAP4(x) (((x>>24) & 0x000000ff) | ((x>>8)  & 0x0000ff00) | ((x<<8)  & 0x00ff0000) | ((x<<24) & 0xff000000))
#define LILEND2(a) (a)
#define LILEND4(a) (a)
#define BIGEND2(a) SWAP2((a))
#define BIGEND4(a) SWAP4((a))

typedef int WORD;
typedef unsigned int DWORD;
typedef char BYTE;



struct AVI_avih
{

    DWORD us_per_frame;
    DWORD max_bytes_per_sec;
    DWORD padding;
    DWORD flags;
    DWORD tot_frames;
    DWORD init_frames;
    DWORD streams;
    DWORD buff_sz;
    DWORD width;
    DWORD height;
    DWORD reserved[4];

};

struct rcFrame
{

    short int left;
    short int top;
    short int right;
    short int bottom;

};

struct AVI_strh
{

    unsigned char type[4];
    unsigned char handler[4];
    DWORD flags;
    DWORD priority;
    DWORD init_frames;
    DWORD scale;
    DWORD rate;
    DWORD start;
    DWORD length;
    DWORD buff_sz;
    DWORD quality;
    DWORD sample_sz;
    struct rcFrame rc;

};

struct AVI_strf
{

    DWORD sz;
    DWORD width;
    DWORD height;
    DWORD planes_bit_cnt;
    unsigned char compression[4];
    DWORD image_sz;
    DWORD xpels_meter;
    DWORD ypels_meter;
    DWORD num_colors;
    DWORD imp_colors;

};

struct AVI_list_hdr
{

    unsigned char id[4];
    DWORD sz;
    unsigned char type[4];

};

struct AVI_list_odml
{

    struct AVI_list_hdr list_hdr;
    unsigned char id[4];
    DWORD sz;
    DWORD frames;

};

struct AVI_list_strl
{

    struct AVI_list_hdr list_hdr;
    unsigned char strh_id[4];
    DWORD strh_sz;
    struct AVI_strh strh;
    unsigned char strf_id[4];
    DWORD strf_sz;
    struct AVI_strf strf;
    struct AVI_list_odml list_odml;

};

struct AVI_list_hdrl
{

    struct AVI_list_hdr list_hdr;
    unsigned char avih_id[4];
    DWORD avih_sz;
    struct AVI_avih avih;
    struct AVI_list_strl strl;

};

typedef struct link{
    int frame;
    long size;
    long offset;
    struct link *pre;
    struct link *next;
}Link;

void create_avi(FILE *fp_avi, int fps, int width, int height);

int write_tail(FILE *fp_avi, int fps, int width, int height);

void add_jpg_to_avi(unsigned char *buf_jpg, FILE *fp_avi,
    unsigned long len_jpg);


#endif
