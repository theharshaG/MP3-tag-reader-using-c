#include <stdio.h>
#include<string.h>
#include "view.h"
#include "edit.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage:\n");
        printf("./a.out -v <mp3 file>\n");
        printf("./a.out -e <mp3 file> -t <new title>\n");
        printf("./a.out -e <mp3 file> -a <new artist>\n");
        printf("./a.out -e <mp3 file> -A <new album>\n");
        printf("./a.out -e <mp3 file> -y <new year>\n");
        printf("./a.out -e <mp3 file> -g <new genre>\n");
        printf("./a.out -e <mp3 file> -c <new comment>\n");

        return 1;
    }

    // View operation 
    if (strcmp(argv[1], "-v") == 0)
    {
        ToviewInfo view;

        if (argc != 3)
        {
            printf("Invalid arguments for view\n");
            return 1;
        }

        if (read_and_validate(argv, &view) == 0)
        {
            printf("Invalid arguments\n");
            return 1;
        }

        if (to_view(&view) == 0)
        {
            return 1;
        }
    }

    //Edit operation
    else if (strcmp(argv[1], "-e") == 0)
    {
        ToeditInfo edit;

        if (argc != 5)
        {
            printf("Invalid arguments for edit\n");
            return 1;
        }

        if (read_and_validate_edit(argv, &edit) == 0)
        {
            printf("Invalid arguments\n");
            return 1;
        }

        if (to_edit(&edit) == 0)
        {
            return 1;
        }
    }

    else
    {
        printf("Invalid option\n");
        printf("Use -v for view or -e for edit\n");

        return 1;
    }

    return 0;
}