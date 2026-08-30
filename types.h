#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

typedef struct
{
    char *mp3_fname;
    FILE *fptr_mp3_file;

    char frame_id[5];

    int size;
    int tag_size;

} ToviewInfo;

#endif