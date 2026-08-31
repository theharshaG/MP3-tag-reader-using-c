#ifndef EDIT_H
#define EDIT_H

#include <stdio.h>

typedef struct
{
    char *mp3_fname;
    char *new_data;
    char *edit_option;
    char *frame_id[5];

    FILE *fptr_mp3_file;

    int tag_size;

} ToeditInfo;

int read_and_validate_edit(char *argv[], ToeditInfo *edit);
int open_files_edit(ToeditInfo *edit);
int check_ID3_tag_edit(ToeditInfo *edit);
void get_frame_id(ToeditInfo *edit);

#endif