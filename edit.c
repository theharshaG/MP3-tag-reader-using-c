#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edit.h"
#include "types.h"

int read_and_validate_edit(char *argv[], ToeditInfo *edit)
{
    //check .mp3 extention 
    if (strstr(argv[2],".mp3")!=NULL)
    {
        edit->mp3_fname = argv[2];
        return 1;
    }
    if (strcmp(argv[3], "-t") != 0 &&
        strcmp(argv[3], "-a") != 0 &&
        strcmp(argv[3], "-A") != 0 &&
        strcmp(argv[3], "-y") != 0 &&
        strcmp(argv[3], "-g") != 0 &&
        strcmp(argv[3], "-c") != 0)
    {
        return 0;
    }

    edit->mp3_fname = argv[2];

    edit->edit_option = argv[3];

    edit->new_data = argv[4];


    return 0;
}
int open_files_edit(ToeditInfo *edit)
{
    edit->fptr_mp3_file = fopen(edit->mp3_fname, "rb+");

    if (edit->fptr_mp3_file == NULL)
    {
        printf("Error: Unable to open file\n");
        return 0;
    }
    

    return 1;
}
int check_ID3_tag_edit(ToeditInfo *edit)
{
    char tag[4];

    rewind(edit->fptr_mp3_file);

    if (fread(tag, 1, 3, edit->fptr_mp3_file) != 3)
    {
        return 0;
    }

    tag[3] = '\0';

    if (strcmp(tag, "ID3") == 0)
    {
        return 1;
    }

    return 0;
}
void get_frame_id(ToeditInfo *edit)
{
    if (strcmp(edit->edit_option, "-t") == 0)
    {
        strcpy(edit->frame_id, "TIT2");
    }
    else if (strcmp(edit->edit_option, "-a") == 0)
    {
        strcpy(edit->frame_id, "TPE1");
    }
    else if (strcmp(edit->edit_option, "-A") == 0)
    {
        strcpy(edit->frame_id, "TALB");
    }
    else if (strcmp(edit->edit_option, "-y") == 0)
    {
        strcpy(edit->frame_id, "TYER");
    }
    else if (strcmp(edit->edit_option, "-g") == 0)
    {
        strcpy(edit->frame_id, "TCON");
    }
    else if (strcmp(edit->edit_option, "-c") == 0)
    {
        strcpy(edit->frame_id, "COMM");
    }
}