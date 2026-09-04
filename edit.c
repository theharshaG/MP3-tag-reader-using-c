#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edit.h"
#include "types.h"


//Validate command line arguments 
int read_and_validate_edit(char *argv[], ToeditInfo *edit)
{
    
    if(strstr(argv[2],".mp3")==NULL)
    {
        return 0;
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

    return 1;
}


// Open original MP3 and temporary file 
int open_files_edit(ToeditInfo *edit)
{
    edit->fptr_mp3_file = fopen(edit->mp3_fname, "rb");

    if (edit->fptr_mp3_file == NULL)
    {
        printf("Error: Unable to open MP3 file\n");
        return 0;
    }

    edit->fptr_temp_file = fopen("temp.mp3", "wb");

    if (edit->fptr_temp_file == NULL)
    {
        printf("Error: Unable to create temporary file\n");

        fclose(edit->fptr_mp3_file);

        return 0;
    }

    return 1;
}


// Check ID3 tag 
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


// Read ID3 version 
void check_version_edit(ToeditInfo *edit)
{
    unsigned char version[2];

    if (fread(version, 1, 2, edit->fptr_mp3_file) != 2)
    {
        return;
    }

    printf("ID3 Version : V2.%d.%d\n",
           version[0],
           version[1]);
}


// Read ID3 tag size 
void read_tag_size_edit(ToeditInfo *edit)
{
    unsigned char size[4];

    //Current position is at byte 5.
    fseek(edit->fptr_mp3_file, 1, SEEK_CUR);  //Skip flags byte.

    if (fread(size, 1, 4, edit->fptr_mp3_file) != 4)
    {
        edit->tag_size = 0;
        return;
    }

    edit->tag_size = 0;

    for (int i = 0; i < 4; i++)
    {
        edit->tag_size = (edit->tag_size << 7) | size[i];
    }

    printf("Tag Size    : %d bytes\n", edit->tag_size);
}


//Convert edit option to frame ID 
void get_frame_id(ToeditInfo *edit)
{
    if (strcmp(edit->edit_option, "-t") == 0)
    {
        strcpy(edit->target_frame, "TIT2");
    }
    else if (strcmp(edit->edit_option, "-a") == 0)
    {
        strcpy(edit->target_frame, "TPE1");
    }
    else if (strcmp(edit->edit_option, "-A") == 0)
    {
        strcpy(edit->target_frame, "TALB");
    }
    else if (strcmp(edit->edit_option, "-y") == 0)
    {
        strcpy(edit->target_frame, "TYER");
    }
    else if (strcmp(edit->edit_option, "-g") == 0)
    {
        strcpy(edit->target_frame, "TCON");
    }
    else if (strcmp(edit->edit_option, "-c") == 0)
    {
        strcpy(edit->target_frame, "COMM");
    }
}


// Copy 10-byte ID3 header 
int copy_header(ToeditInfo *edit)
{
    unsigned char header[10];

    rewind(edit->fptr_mp3_file);

    if (fread(header, 1, 10, edit->fptr_mp3_file) != 10)
    {
        printf("Error: Unable to read ID3 header\n");
        return 0;
    }

    if (fwrite(header, 1, 10, edit->fptr_temp_file) != 10)
    {
        printf("Error: Unable to write ID3 header\n");
        return 0;
    }

    return 1;
}


// Read one frame header 
int read_frame_header(ToeditInfo *edit)
{
    unsigned char size[4];

    // Read frame ID.

    if (fread(edit->frame_id, 1, 4,
              edit->fptr_mp3_file) != 4)
    {
        return 0;
    }

    edit->frame_id[4] = '\0';

    
     //Empty frame means padding/end of frames.
    
    if (edit->frame_id[0] == '\0')
    {
        return 0;
    }

    
    //Read frame size.
    if (fread(size, 1, 4,
              edit->fptr_mp3_file) != 4)
    {
        return 0;
    }

    edit->size = 0;


    //ID3v2.3 frame size is big-endian.
    for (int i = 0; i < 4; i++)
    {
        edit->size = (edit->size << 8) | size[i];
    }

    
    //Read 2 bytes of frame flags.
    if (fread(edit->flags, 1, 2,
              edit->fptr_mp3_file) != 2)
    {
        return 0;
    }

    return 1;
}


// Write frame header
void write_frame_header(ToeditInfo *edit, int size)
{
    unsigned char buffer[4];

    /*
       Convert integer size into
       4-byte big-endian format.
    */
    buffer[0] = (size >> 24) & 0xFF;
    buffer[1] = (size >> 16) & 0xFF;
    buffer[2] = (size >> 8) & 0xFF;
    buffer[3] = size & 0xFF;

    //rite frame ID.
    fwrite(edit->frame_id, 1, 4,
           edit->fptr_temp_file);

    //Write frame size.
    fwrite(buffer, 1, 4,
           edit->fptr_temp_file);

   //Write frame flags.
    fwrite(edit->flags, 1, 2,
           edit->fptr_temp_file);
}


//Copy unchanged frame data 
int copy_frame_data(ToeditInfo *edit)
{
    unsigned char ch;

    for (int i = 0; i < edit->size; i++)
    {
        if (fread(&ch, 1, 1,
                  edit->fptr_mp3_file) != 1)
        {
            return 0;
        }

        if (fwrite(&ch, 1, 1,
                   edit->fptr_temp_file) != 1)
        {
            return 0;
        }
    }

    return 1;
}


//Write new frame data 
int write_new_frame_data(ToeditInfo *edit)
{
    int new_size;

    /*
       One byte for encoding
       + number of characters.
    */
    new_size = strlen(edit->new_data) + 1;

    /*
       Write encoding byte.
       0 = ISO-8859-1.
    */
    fputc(0, edit->fptr_temp_file);

    /*
       Write new text.
    */
    fwrite(edit->new_data, 1,
           strlen(edit->new_data),
           edit->fptr_temp_file);

    return new_size;
}


//process one frame
int process_frame(ToeditInfo *edit)
{
    int old_size;
    int new_size;

    //Read frame header from original file.
    if (read_frame_header(edit) == 0)
    {
        return 0;
    }

    //Save original frame size.
    old_size = edit->size;

    /*
       Check whether this is the frame
       that we want to edit.
    */
    if (strcmp(edit->frame_id,
               edit->target_frame) == 0)
    {
        /*
           Calculate new frame size.

           1 byte = encoding
           remaining = new text
        */
        new_size = strlen(edit->new_data) + 1;

        
        //Write new frame header.
        write_frame_header(edit, new_size);

        //Write new frame data.
        write_new_frame_data(edit);

        //Skip old frame data in original file.
        fseek(edit->fptr_mp3_file,
              old_size,
              SEEK_CUR);

        edit->found = 1;
    }
    else
    {
        //Copy frame header unchanged.
        write_frame_header(edit, old_size);

        //Copy frame data unchanged.
        if (copy_frame_data(edit) == 0)
        {
            return 0;
        }
    }

    return 1;
}


//Process all ID3 frames 
int process_all_frames(ToeditInfo *edit)
{
    int bytes_read = 0;

    while (bytes_read < edit->tag_size)
    {
        
        //Process one frame
        if (process_frame(edit) == 0)
        {
            break;
        }

        /*
           Count bytes consumed from original file.

           Frame header = 10 bytes
           Frame data   = old_size bytes
        */
        bytes_read = bytes_read + 10 + edit->size;
    }

    return 1;
}


//Copy remaining MP3 audio data
int copy_remaining_data(ToeditInfo *edit)
{
    unsigned char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer),
                          edit->fptr_mp3_file)) > 0)
    {
        if (fwrite(buffer, 1, bytes,
                   edit->fptr_temp_file) != bytes)
        {
            return 0;
        }
    }

    return 1;
}


//Replace original MP3 with temporary file
int replace_original_file(ToeditInfo *edit)
{
    //Close both files before remove/rename.
    fclose(edit->fptr_mp3_file);
    fclose(edit->fptr_temp_file);

    //Delete original MP3.
    if (remove(edit->mp3_fname) != 0)
    {
        printf("Error: Unable to remove original file\n");

        remove("temp.mp3");

        return 0;
    }

    //Rename temp.mp3 to original filename.
    if (rename("temp.mp3", edit->mp3_fname) != 0)
    {
        printf("Error: Unable to rename temporary file\n");
        return 0;
    }

    return 1;
}


//Complete edit operation 
int to_edit(ToeditInfo *edit)
{
    //Open original and temporary files.
    if (open_files_edit(edit) == 0)
    {
        return 0;
    }

    //Check ID3 tag.
    if (check_ID3_tag_edit(edit) == 0)
    {
        printf("Invalid MP3 File\n");

        fclose(edit->fptr_mp3_file);
        fclose(edit->fptr_temp_file);

        remove("temp.mp3");

        return 0;
    }

    printf("\n------ MP3 Tag Editor ------\n\n");

    //Read version.
    check_version_edit(edit);

    //Read tag size.
    read_tag_size_edit(edit);

    //Convert option to frame ID.
    get_frame_id(edit);

    printf("Editing Frame : %s\n", edit->target_frame);
    printf("New Data      : %s\n\n", edit->new_data);

    //Copy original 10-byte ID3 header.
    if (copy_header(edit) == 0)
    {
        fclose(edit->fptr_mp3_file);
        fclose(edit->fptr_temp_file);

        remove("temp.mp3");

        return 0;
    }

    
    //Process all ID3 frames.
    process_all_frames(edit);

    //Check whether target frame was found.
    if (edit->found == 0)
    {
        printf("Frame %s not found\n", edit->target_frame);

        fclose(edit->fptr_mp3_file);
        fclose(edit->fptr_temp_file);

        remove("temp.mp3");

        return 0;
    }

    //Copy remaining MP3 audio data.
    
    if (copy_remaining_data(edit) == 0)
    {
        printf("Error while copying MP3 audio data\n");

        fclose(edit->fptr_mp3_file);
        fclose(edit->fptr_temp_file);

        remove("temp.mp3");

        return 0;
    }

    //Replace original file with temp file.
    if (replace_original_file(edit) == 0)
    {
        return 0;
    }

    printf("Tag updated successfully\n");
    printf("----------------------------\n");

    return 1;
}