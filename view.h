#ifndef VIEW_H
#define VIEW_H

#include "types.h"

int read_and_validate(char *argv[], ToviewInfo *view);
int open_files(ToviewInfo *view);
int check_ID3_tag(ToviewInfo *view);

void check_version(ToviewInfo *view);
void read_tag_size(ToviewInfo *view);
void skip_header(ToviewInfo *view);

void read_frame_id(ToviewInfo *view);
void read_size(ToviewInfo *view);
void skip_flags(ToviewInfo *view);
void read_contents(ToviewInfo *view);

void mp3_view(ToviewInfo *view);

int to_view(ToviewInfo *view);

#endif