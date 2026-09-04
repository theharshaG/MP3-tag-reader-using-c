#ifndef EDIT_H
#define EDIT_H

#include <stdio.h>

typedef struct
{
    char *mp3_fname;
    char *new_data;
    char *edit_option;

    char frame_id[5];
    char target_frame[5];

    unsigned char flags[2];

    FILE *fptr_mp3_file;
    FILE *fptr_temp_file;

    int tag_size;
    int size;
    int found;

} ToeditInfo;


int read_and_validate_edit(char *argv[], ToeditInfo *edit);

int open_files_edit(ToeditInfo *edit);

int check_ID3_tag_edit(ToeditInfo *edit);

void check_version_edit(ToeditInfo *edit);

void read_tag_size_edit(ToeditInfo *edit);

void get_frame_id(ToeditInfo *edit);

int copy_header(ToeditInfo *edit);

int read_frame_header(ToeditInfo *edit);

void write_frame_header(ToeditInfo *edit, int size);

int copy_frame_data(ToeditInfo *edit);

int write_new_frame_data(ToeditInfo *edit);

int process_frame(ToeditInfo *edit);

int process_all_frames(ToeditInfo *edit);

int copy_remaining_data(ToeditInfo *edit);

int replace_original_file(ToeditInfo *edit);

int to_edit(ToeditInfo *edit);

#endif