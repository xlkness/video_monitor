#include "monitor_avi_util.h"

struct AVI_list_hdrl hdrl;

typedef unsigned long off64_t;

Link *h;

#define JPEG_DATA_SZ (sizeof(DWORD) * 2)
#define PATH_MAX 255

static const DWORD AVIF_HASINDEX = 0x00000010;
static const DWORD AVIF_MUSTUSEINDEX = 0x00000020;
static const DWORD AVIF_ISINTERLEAVED = 0x00000100;
static const DWORD AVIF_TRUSTCKTYPE = 0x00000800;
static const DWORD AVIF_WASCAPTUREFILE = 0x00010000;
static const DWORD AVIF_COPYRIGHTED = 0x00020000;

static void insert_node(Link *h, int frame, long size, long offset)
{
    Link *node;
    node = (Link *)malloc(sizeof(Link));

    node->frame = frame;
    node->size = size;
    node->offset = offset;

    if(h->next == NULL)
    {
        h->next = node;
        h->pre = node;

        node->pre = h;
        node->next = h;
    }
    else
    {
        node->next = h->next;
        node->pre = h;

        h->next->pre = node;
        h->next = node;
    }
/*
    Link *p;
    p = h->next;
    while(1){
        if(p==h) break;
        printf("p->size:%d,p->offset:%d,p->frame:%d\n",
            p->size,p->offset,p->frame);
        p = p->next;
    }
    printf("\n");
*/
}
void free_node()
{
    Link *p;
    p = h->pre;
    while(p!=h){
        p->pre->next = h;
        h->pre = p->pre;
        free(p);
        p = h->pre;
    }
    free(h);
}
void print_quartet(unsigned int i, FILE *out)
{

    putc(i % 0x100, out);
    i /= 0x100;
    putc(i % 0x100, out);
    i /= 0x100;
    putc(i % 0x100, out);
    i /= 0x100;
    putc(i % 0x100, out);

}
int fill_avi_header(FILE *fp_avi, unsigned int fps,
    unsigned int width, unsigned int height)
{
    char tempstr[255];
    DWORD per_usec = 1;
    DWORD frames = 1;

    off64_t jpg_sz_64 = 1, riff_sz_64 = 1;
    long jpg_sz = 1;
    const off64_t MAX_RIFF_SZ = 2147483648UL; // 2 GB limit
    DWORD riff_sz = 0;

    int fd;
    long nbr, nbw, tnbw = 0;
    char buff[512];
    off_t mfsz, remnant;

    hdrl = (struct AVI_list_hdrl)
    {

        {

            {
                'L', 'I', 'S', 'T'
            },
            LILEND4(sizeof(struct AVI_list_hdrl) - 8),
            {
                'h', 'd', 'r', 'l'
            }

        },
        {
            'a', 'v', 'i', 'h'
        },
        LILEND4(sizeof(struct AVI_avih)),
        {

            LILEND4(per_usec),
            LILEND4(1000000 * (jpg_sz/frames) / per_usec),
            LILEND4(0),
            LILEND4(AVIF_HASINDEX),
            LILEND4(frames),
            LILEND4(0),
            LILEND4(1),
            LILEND4(0),
            LILEND4(width),
            LILEND4(height),
            {
                LILEND4(0), LILEND4(0), LILEND4(0), LILEND4(0)
            }

        },
        {

            {

                {
                    'L', 'I', 'S', 'T'
                },
                LILEND4(sizeof(struct AVI_list_strl) - 8),
                {
                    's', 't', 'r', 'l'
                }

            },
            {
                's', 't', 'r', 'h'
            },
            LILEND4(sizeof(struct AVI_strh)),
            {

                {
                    'v', 'i', 'd', 's'
                },
                {
                    'M', 'J', 'P', 'G'
                },
                LILEND4(0),
                LILEND4(0),
                LILEND4(0),
                LILEND4(per_usec),
                LILEND4(1000000),
                LILEND4(0),
                LILEND4(frames),
                LILEND4(0),
                LILEND4(0),
                LILEND4(0),
                LILEND2(0),
                LILEND2(0),
                LILEND2(160),
                LILEND2(120)

            },
            {
                's', 't', 'r', 'f'
            },
            sizeof(struct AVI_strf),
            {

                LILEND4(sizeof(struct AVI_strf)),
                LILEND4(width),
                LILEND4(height),
                LILEND4(1 + (24<<16)),
                {
                    'M', 'J', 'P', 'G'
                },
                LILEND4(width * height * 3),
                LILEND4(0),
                LILEND4(0),
                LILEND4(0),
                LILEND4(0)

            },
            {

                {

                    {
                        'L', 'I', 'S', 'T'
                    },
                    LILEND4(16),
                    {
                        'o', 'd', 'm', 'l'
                    }

                },
                {
                    'd', 'm', 'l', 'h'
                },
                LILEND4(4),
                LILEND4(frames)

            }

        }

    };

    per_usec = 1000000/fps;

    putc('R', fp_avi);
    putc('I', fp_avi);
    putc('F', fp_avi);
    putc('F', fp_avi);
    /*写这个riff_sz，64位，到文件中*/
    print_quartet(riff_sz, fp_avi);
    putc('A', fp_avi);
    putc('V', fp_avi);
    putc('I', fp_avi);
    putc(' ', fp_avi);
    hdrl.avih.us_per_frame = LILEND4(per_usec);

    hdrl.avih.max_bytes_per_sec = LILEND4(per_usec);//

    hdrl.avih.tot_frames = LILEND4(frames);
    hdrl.avih.width = LILEND4(width);
    hdrl.avih.height = LILEND4(height);
    hdrl.strl.strh.scale = LILEND4(per_usec);
    hdrl.strl.strh.rate = LILEND4(1000000);

    hdrl.strl.strh.length = LILEND4(frames);

    hdrl.strl.strf.width = LILEND4(width);
    hdrl.strl.strf.height = LILEND4(height);
    hdrl.strl.strf.image_sz = LILEND4(width * height * 3);
    hdrl.strl.list_odml.frames = LILEND4(frames);
    fwrite(&hdrl, sizeof(hdrl), 1, fp_avi);
    putc('L', fp_avi);
    putc('I', fp_avi);
    putc('S', fp_avi);
    putc('T', fp_avi);

    print_quartet(jpg_sz + 8*frames + 4, fp_avi);

    putc('m', fp_avi);
    putc('o', fp_avi);
    putc('v', fp_avi);
    putc('i', fp_avi);
}
int avi_add_jpg(Link *h, unsigned char *buf_jpg, FILE *fp_avi,
    unsigned long len_jpg)
{
    unsigned int remnant;
    int frame = 0;
    long offset;

    if(h->next == NULL)
    {
        frame += 1;
        offset = 4;
    }
    else
    {
        offset = h->next->offset + h->next->size + 8;
        frame = (h->next->frame)+1;
    }


    putc('0', fp_avi);
    putc('0', fp_avi);
    putc('d', fp_avi);
    putc('c', fp_avi);

    remnant = (4-(len_jpg%4)) % 4;
    print_quartet(len_jpg + remnant, fp_avi);

    insert_node(h, frame, len_jpg+remnant, offset);

    fwrite(buf_jpg, 1, 6, fp_avi);
    fwrite("AVI1", 4, 1, fp_avi);

    fwrite(buf_jpg+10, 1, len_jpg-10, fp_avi);

    if (remnant > 0)
    {
        fwrite(buf_jpg+10, remnant, 1, fp_avi);
    }
}
void add_jpg_to_avi(unsigned char *buf_jpg, FILE *fp_avi,
    unsigned long len_jpg)
{
/*
    FILE *fp;
    char a;
    unsigned char *path;
    path = (unsigned char *)malloc(10);
    if(i == 9){
        a = '9';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }

    if(i == 8)
    {
        a = '8';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }

    if(i == 7)
    {
        a = '7';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }

    if(i == 6)
    {
        a = '6';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }

    if(i == 5)
    {
        a = '5';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }

    if(i == 4)
    {
        a = '4';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }

    if(i == 3)
    {
        a = '3';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }

    if(i == 2)
    {
        a = '2';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }

    if(i == 1)
    {
        a = '1';
        memcpy(path, &a, 1);
        memcpy(path+1, ".jpg", 4);
        fp = fopen(path, "wb+");
        fwrite(buf_jpg, 1, len_jpg, fp);
        fclose(fp);
    }
*/
    avi_add_jpg(h, buf_jpg, fp_avi, len_jpg);
}
int write_tail(FILE *fp_avi, int fps, int width, int height)
{
    int i;
    DWORD per_usec = 1;
    Link *p = h->next;

    unsigned long len_all_jpg = 0;
    int frames = h->next->frame;
    while(p!=h){
        len_all_jpg += p->size;
        p = p->next;
    }
    unsigned long avi_len = sizeof(struct AVI_list_hdrl) + 4 +
        4 + len_all_jpg + 8*frames + 8 + 8 + 16*frames;
    printf("alllenjpg:%lu,fps:%d,frames:%d,width:%d,height:%d\n",avi_len,fps,frames,width,height);
    /*write tail*/
    putc('i', fp_avi);
    putc('d', fp_avi);
    putc('x', fp_avi);
    putc('1', fp_avi);
    print_quartet(16 * frames, fp_avi);

    for (p=h->pre; p!=h; p=p->pre)
    {

        putc('0', fp_avi);
        putc('0', fp_avi);
        putc('d', fp_avi);
        putc('c', fp_avi);
        print_quartet(18, fp_avi);

        print_quartet(p->offset, fp_avi);
        print_quartet(p->size, fp_avi);
    }

    /*write header*/
    per_usec = 1000000/fps;

    fseek(fp_avi, 0, SEEK_SET);

    hdrl = (struct AVI_list_hdrl)
    {

        {

            {
                'L', 'I', 'S', 'T'
            },
            LILEND4(sizeof(struct AVI_list_hdrl) - 8),
            {
                'h', 'd', 'r', 'l'
            }

        },
        {
            'a', 'v', 'i', 'h'
        },
        LILEND4(sizeof(struct AVI_avih)),
        {

            LILEND4(per_usec),
            LILEND4(1000000 * (len_all_jpg/frames) / per_usec),
            LILEND4(0),
            LILEND4(AVIF_HASINDEX),
            LILEND4(frames),
            LILEND4(0),
            LILEND4(1),
            LILEND4(0),
            LILEND4(width),
            LILEND4(height),
            {
                LILEND4(0), LILEND4(0), LILEND4(0), LILEND4(0)
            }

        },
        {

            {

                {
                    'L', 'I', 'S', 'T'
                },
                LILEND4(sizeof(struct AVI_list_strl) - 8),
                {
                    's', 't', 'r', 'l'
                }

            },
            {
                's', 't', 'r', 'h'
            },
            LILEND4(sizeof(struct AVI_strh)),
            {

                {
                    'v', 'i', 'd', 's'
                },
                {
                    'M', 'J', 'P', 'G'
                },
                LILEND4(0),
                LILEND4(0),
                LILEND4(0),
                LILEND4(per_usec),
                LILEND4(1000000),
                LILEND4(0),
                LILEND4(frames),
                LILEND4(0),
                LILEND4(0),
                LILEND4(0),
                LILEND2(0),
                LILEND2(0),
                LILEND2(160),
                LILEND2(120)

            },
            {
                's', 't', 'r', 'f'
            },
            sizeof(struct AVI_strf),
            {

                LILEND4(sizeof(struct AVI_strf)),
                LILEND4(width),
                LILEND4(height),
                //11000<<16:110000000000000000001
                //LILEND4(1 + (24<<16)),
                LILEND4(1 + (24<<16)),
                {
                    'M', 'J', 'P', 'G'
                },
                LILEND4(width * height * 2),
                LILEND4(0),
                LILEND4(0),
                LILEND4(0),
                LILEND4(0)

            },
            {

                {

                    {
                        'L', 'I', 'S', 'T'
                    },
                    LILEND4(16),
                    {
                        'o', 'd', 'm', 'l'
                    }

                },
                {
                    'd', 'm', 'l', 'h'
                },
                LILEND4(4),
                LILEND4(frames)

            }

        }

    };

    putc('R', fp_avi);
    putc('I', fp_avi);
    putc('F', fp_avi);
    putc('F', fp_avi);

    print_quartet(avi_len, fp_avi);
    putc('A', fp_avi);
    putc('V', fp_avi);
    putc('I', fp_avi);
    putc(' ', fp_avi);
    hdrl.avih.us_per_frame = LILEND4(per_usec);
    printf("lenalljpg:%lu,persec:%ld,frames:%d,width:%d,height:%d\n",
        len_all_jpg,per_usec,frames,width,height);
    hdrl.avih.max_bytes_per_sec =
        LILEND4(1000000 * (len_all_jpg/frames) / per_usec);//

    hdrl.avih.tot_frames = LILEND4(frames);
    hdrl.avih.width = LILEND4(width);
    hdrl.avih.height = LILEND4(height);
    hdrl.strl.strh.scale = LILEND4(per_usec);
    hdrl.strl.strh.rate = LILEND4(1000000);

    hdrl.strl.strh.length = LILEND4(frames);

    hdrl.strl.strf.width = LILEND4(width);
    hdrl.strl.strf.height = LILEND4(height);
    hdrl.strl.strf.image_sz = LILEND4(width * height * 2);
    hdrl.strl.list_odml.frames = LILEND4(frames);
    fwrite(&hdrl, sizeof(hdrl), 1, fp_avi);
    putc('L', fp_avi);
    putc('I', fp_avi);
    putc('S', fp_avi);
    putc('T', fp_avi);

    print_quartet(len_all_jpg + 8*frames + 4, fp_avi);


    free_node();
}

void create_avi(FILE *fp_avi, int fps, int width, int height)
{
    h = (Link *)malloc(sizeof(Link));
    h->next = NULL;
    h->pre = NULL;
    h->frame = 0;

    fill_avi_header(fp_avi, fps, width, height);
}
