#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "view.h"
#include "types.h"


// Validate command line arguments 
int read_and_validate(char *argv[], ToviewInfo *view)
{
    char *ext;

    //Check -v option
    if (strcmp(argv[1], "-v") != 0)
    {
        return 0;
    }

    if( strstr(argv[2],".mp3") == 0)
    {
        view->mp3_fname = argv[2];
        return 1;
    }

    return 0;
}


// Open MP3 file
int open_files(ToviewInfo *view)
{
    view->fptr_mp3_file = fopen(view->mp3_fname, "rb");

    if (view->fptr_mp3_file == NULL)
    {
        printf("Error: Unable to open file\n");
        return 0;
    }

    return 1;
}


// Check ID3 tag 
int check_ID3_tag(ToviewInfo *view)
{
    char tag[4];

    rewind(view->fptr_mp3_file);

    if (fread(tag, 1, 3, view->fptr_mp3_file) != 3)
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


// Read ID3 version and revision 
void check_version(ToviewInfo *view)
{
    unsigned char version[2];

    fread(version, 1, 2, view->fptr_mp3_file);

    printf("ID3 Version : V2.%d.%d\n",
           version[0],
           version[1]);
}


//Read ID3 tag size 
void read_tag_size(ToviewInfo *view)
{
    unsigned char size[4];

    fseek(view->fptr_mp3_file, 1, SEEK_CUR);// Move file pointer 1 byte forward
                                             // Skip the flags byte

    fread(size, 1, 4, view->fptr_mp3_file);

    view->tag_size = 0;

    for(int i = 0; i < 4; i++)
    {
        view->tag_size = (view->tag_size << 7) | size[i];
    }

    printf("Tag Size : %d\n", view->tag_size);
}


//Move file pointer to first frame
void skip_header(ToviewInfo *view)
{
    fseek(view->fptr_mp3_file, 10, SEEK_SET);
}


// read frame ID
void read_frame_id(ToviewInfo *view)
{
    fread(view->frame_id, 1, 4, view->fptr_mp3_file); // Read 4 bytes of frame [like ID : T I T 2 '\0']
    view->frame_id[4] = '\0'; // Add null character
}


//read frame size 
void read_size(ToviewInfo *view)
{
    unsigned char size[4];

    fread(size, 1, 4, view->fptr_mp3_file); // Read 4 bytes of frame size

    view->size = 0;

    for(int i = 0; i < 4; i++)
    {
        view->size = (view->size << 8) | size[i]; // Combine bytes into one size
    }
}


//Skip frame flags 
void skip_flags(ToviewInfo *view)
{
    fseek(view->fptr_mp3_file, 2, SEEK_CUR);  // Skip 2 bytes of frame flags
}


// Read frame contents
void read_contents(ToviewInfo *view)
{
    unsigned char encoding;
    char *data;

    if (view->size <= 0)
    {
        return;
    }

    data = malloc(view->size);       // Allocate memory according to frame size.


    if (data == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    fread(&encoding, 1, 1, view->fptr_mp3_file);   //First byte is encoding.

    fread(data, 1, view->size - 1, view->fptr_mp3_file);  //Read remaining data.

    data[view->size - 1] = '\0';


    if (strcmp(view->frame_id, "TIT2") == 0)
    {
        printf("Title   : %s\n", data);
    }
    else if (strcmp(view->frame_id, "TPE1") == 0)
    {
        printf("Artist  : %s\n", data);
    }
    else if (strcmp(view->frame_id, "TALB") == 0)
    {
        printf("Album   : %s\n", data);
    }
    else if (strcmp(view->frame_id, "TYER") == 0)
    {
        printf("Year    : %s\n", data);
    }
    else if (strcmp(view->frame_id, "TCON") == 0)
    {
        printf("Genre   : %s\n", data);
    }
    else if (strcmp(view->frame_id, "COMM") == 0)
    {
        printf("Comment : %s\n", data);
    }

    free(data);
}


// Read all ID3 frames 
void mp3_view(ToviewInfo *view)
{
    int bytes_read = 0;

    skip_header(view);  // Move file pointer to first frame

    while (bytes_read < view->tag_size)
    {
        // Read 4 bytes of frame ID
        if (fread(view->frame_id, 1, 4, view->fptr_mp3_file) != 4)
        {
            break;
        }

        view->frame_id[4] = '\0';  // Add null character

        // Stop if empty frame is found
        if (view->frame_id[0] == '\0')
        {
            break;
        }

        read_size(view);  // Read frame size

        skip_flags(view);  // Skip 2 bytes of flags

        bytes_read = bytes_read + 10;  // Frame header = 10 bytes

        read_contents(view);  // Read frame data

        bytes_read = bytes_read + view->size;  // Add frame data size
    }
}

//complete read operation
int to_view(ToviewInfo *view)
{
    if (open_files(view) == 0)
    {
        return 0;
    }

    if (check_ID3_tag(view) == 0)
    {
        printf("Invalid MP3 File\n");

        fclose(view->fptr_mp3_file);

        return 0;
    }

    printf("\n------ MP3 Tag Reader ------\n\n");

    check_version(view);

    read_tag_size(view);

    printf("\n");

    mp3_view(view);

    printf("\n----------------------------\n");

    fclose(view->fptr_mp3_file);

    return 1;
}