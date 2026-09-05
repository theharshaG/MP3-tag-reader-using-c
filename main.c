#include <stdio.h>
#include <string.h>
#include "view.h"
#include "edit.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage:\n");
        printf("./mp3_tag_reader -v <file.mp3>\n");
        printf("./mp3_tag_reader -e <file.mp3> <option> <value>\n");
        printf("./mp3_tag_reader -h\n");

        return 1;
    }

    //View operation
    if (strcmp(argv[1], "-v") == 0)
    {
        ToviewInfo view;

        if (argc != 3)
        {
            printf("Invalid arguments for view\n");
            printf("Usage: ./mp3_tag_reader -v <file.mp3>\n");
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

    // Edit operation
    else if (strcmp(argv[1], "-e") == 0)
    {
        ToeditInfo edit;

        if (argc != 5)
        {
            printf("Invalid arguments for edit\n");
            printf("Usage: ./mp3_tag_reader -e <file.mp3> <option> <value>\n");
            printf("\nOptions:\n");
            printf("  -t <value>    Modify Title\n");
            printf("  -A <value>    Modify Artist\n");
            printf("  -a <value>    Modify Album\n");
            printf("  -y <value>    Modify Year\n");
            printf("  -m <value>    Modify Content Type\n");
            printf("  -c <value>    Modify Composer\n");

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

    //Help operation 
    else if (strcmp(argv[1], "-h") == 0)
    {
        if (argc != 2)
        {
            printf("ERROR: Invalid arguments for help\n");
            printf("Usage: ./mp3_tag_reader -h\n");

            return 1;
        }

        printf("\n=====================================\n");
        printf("       MP3 TAG READER AND EDITOR\n");
        printf("=====================================\n");

        printf("\nUsage:\n");

        printf("\nView MP3 tags:\n");
        printf("  ./mp3_tag_reader -v <file.mp3>\n");

        printf("\nEdit MP3 tags:\n");
        printf("  ./mp3_tag_reader -e <file.mp3> <option> <value>\n");

        printf("\nEdit Options:\n");
        printf("  -t <value>    Modify Title\n");
        printf("  -A <value>    Modify Artist name\n");
        printf("  -a <value>    Modify Album name\n");
        printf("  -y <value>    Modify Year\n");
        printf("  -m <value>    Modify Content type\n");
        printf("  -c <value>    Modify Composer\n");

        printf("\nHelp:\n");
        printf("  ./mp3_tag_reader -h\n");

        printf("\n=====================================\n");
    }

    //Invalid operation 
    else
    {
        printf("ERROR: Invalid option '%s'\n", argv[1]);

        printf("\nValid options:\n");
        printf("  ./mp3_tag_reader -v <file.mp3>\n");
        printf("  ./mp3_tag_reader -e <file.mp3> <option> <value>\n");
        printf("  ./mp3_tag_reader -h\n");

        return 1;
    }

    return 0;
}