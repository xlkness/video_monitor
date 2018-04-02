#ifndef _MONITOR_JPG_UTIL_H
#define _MONITOR_JPG_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor_huffman.h"

int create_jpg(unsigned char *out_buf,
                       unsigned char *in_buf,
                       int in_size);

#endif
