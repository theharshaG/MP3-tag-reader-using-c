#include <stdio.h>
#include<string.h>

#include "view.h"
#include "edit.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid arguments\n");
        return 1;
    }

    if (strcmp(argv[1], "-v") == 0)
    {
        ToviewInfo view;
        if (argc != 3)
        {
            printf("Usage: %s -v <mp3 file>\n", argv[0]);
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
    else if (strcmp(argv[1], "-e") == 0)
    {
        ToeditInfo edit;
        if(argc != 5)
        {
            printf("Usage: %s -e <mp3 file> -t <new title>\n", argv[0]);
            return 1;
        }

        if(read_and_validate_edit(argv, &edit)==0)
        {
            printf("Invalid arguments\n");
            return 0;
        }
    }
    else
    {
        printf("Invalid operation\n");
        return 1;
    }

    return 0;
}