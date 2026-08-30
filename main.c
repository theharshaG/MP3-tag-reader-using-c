#include <stdio.h>

#include "view.h"
#include "types.h"

int main(int argc, char *argv[])
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

    return 0;
}