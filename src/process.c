#include "process.h"
#include <errno.h>     // error descriptors
#include <stdio.h>     // printf
#include <stdlib.h>    // EXIT codes
#include <unistd.h>    // file read, write

int process(int fd_input, int fd_output, const char *filename_input, filter_func filter)
{
    char letter[2] = {' ', '\0'};

    do
    {
        const ssize_t read_ret = read(fd_input, &letter, 1);

        if(read_ret == 0)
        {
            break;    // EOF
        }

        if(read_ret == -1)
        {
            if(errno == EISDIR)
            {
                printf("\"%s\" is a directory, therefore it can not be read.\n", filename_input);
            }
            else
            {
                printf("Reading error.\n");
            }

            return EXIT_FAILURE;
        }

        filter(letter);

        write(fd_output, &letter, 1);

    } while(*letter != '\0');

    return EXIT_SUCCESS;
}
