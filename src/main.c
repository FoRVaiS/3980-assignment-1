#include "filters.h"
#include "process.h"
#include <errno.h>     // errno descriptors
#include <fcntl.h>     // open() flags
#include <stdio.h>     // [io] printf, fd(1, 2, 3)
#include <stdlib.h>    // EXIT codes
#include <unistd.h>    // getopt

#define USER_OPEN_PERMS 0644

int main(int argc, char *argv[])
{
    int opt = -1;

    int fd_input  = -1;
    int fd_output = -1;

    const char *filename_input  = NULL;
    const char *filename_output = NULL;
    char       *filter_name     = NULL;
    filter_func filter          = NULL;

    int process_ret = EXIT_SUCCESS;

    while((opt = getopt(argc, argv, "i:o:f:h")) != -1)
    {
        switch(opt)
        {
            case 'f':
                filter_name = optarg;
                break;
            case 'i':
                filename_input = optarg;
                break;
            case 'o':
                filename_output = optarg;
                break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s -i <input_path> -o <output_path> -f <\"upper\", \"lower\", \"null\">\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // INPUT VALIDATION
    if(filename_input == NULL)
    {
        printf("Missing input file option.\n");
        return EXIT_FAILURE;
    }

    if(filename_output == NULL)
    {
        printf("Missing output file option.\n");
        return EXIT_FAILURE;
    }

    if(filter_name == NULL)
    {
        printf("Missing filter option.\n");
        return EXIT_FAILURE;
    }

    errno    = 0;
    fd_input = open(filename_input, O_RDONLY | O_CLOEXEC);
    if(fd_input == -1)
    {
        if(errno == ENOENT)
        {
            printf("Input file: \"%s\" does not exist.\n", filename_input);
        }
        else
        {
            printf("Failed to open input file: %s\n", filename_input);
        }

        return EXIT_FAILURE;
    }

    errno     = 0;
    fd_output = open(filename_output, O_CREAT | O_WRONLY | O_CLOEXEC, USER_OPEN_PERMS);
    if(fd_output == -1)
    {
        if(errno == EISDIR)
        {
            printf("\"%s\" is a directory, therefore can not be written to.\n", filename_output);
        }
        else
        {
            printf("Failed to open output file: %s\n", filename_output);
        }

        return EXIT_FAILURE;
    }

    filter = use_filter(filter_name);
    if(filter == NULL)
    {
        return EXIT_FAILURE;
    }

    process_ret = process(fd_input, fd_output, filename_input, filter);

    close(fd_input);
    close(fd_output);

    return process_ret;
}
