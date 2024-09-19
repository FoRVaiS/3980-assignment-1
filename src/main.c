#include "filters.h"
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define USER_OPEN_PERMS 0644

typedef unsigned long ulong;
typedef void (*filter_func)(char *str);

int main(int argc, char *argv[])
{
    int  opt       = -1;
    int  fd_input  = -1;
    int  fd_output = -1;
    char letter[2] = {' ', '\0'};

    const char *filename_input  = NULL;
    const char *filename_output = NULL;
    char       *filter          = NULL;
    filter_func transformer     = NULL;

    while((opt = getopt(argc, argv, "i:o:f:")) != -1)
    {
        switch(opt)
        {
            case 'f':
                filter = optarg;
                break;
            case 'i':
                filename_input = optarg;
                break;
            case 'o':
                filename_output = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-a] [-b value] [-c]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // INPUT VALIDATION
    if(filename_input == NULL)
    {
        printf("You must supply a valid filepath to your input file");
        return EXIT_FAILURE;
    }

    if(filename_output == NULL)
    {
        printf("You must supply a valid filepath to the desired output file");
        return EXIT_FAILURE;
    }

    if(filter == NULL)
    {
        printf("You must choose at least one of three filters: upper, lower, null");
        return EXIT_FAILURE;
    }

    filter_lower(filter);    // Convert the filter selector to lowercase

    if(strcmp(filter, "upper") == 0)
    {
        transformer = &filter_upper;
    }
    if(strcmp(filter, "lower") == 0)
    {
        transformer = &filter_lower;
    }
    if(strcmp(filter, "null") == 0)
    {
        transformer = &filter_none;
    }

    // These is a case where the user enters an invalid filter
    // and the filter won't be selected
    if(transformer == NULL)
    {
        printf("\"%s\" is not a valid filter, please choose one of three valid filters: upper, lower, null", filter);
        return EXIT_FAILURE;
    }

    errno    = 0;
    fd_input = open(filename_input, O_RDONLY | O_CLOEXEC);
    if(fd_input == -1)
    {
        if(errno == ENOENT)
        {
            printf("\"%s\" does not exist.\n", filename_input);
        }
        else if(errno == EISDIR)
        {
            printf("\"%s\" is a directory, but should be a file.\n", filename_input);
        }
        else
        {
            printf("Failed to open file: %s\n", filename_input);
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
            printf("Failed to open file: %s\n", filename_output);
        }

        return EXIT_FAILURE;
    }

    do
    {
        const ssize_t read_ret = read(fd_input, &letter, 1);
        transformer(letter);
        write(fd_output, &letter, 1);

        if(read_ret == 0)
        {
            break;    // EOF
        }

        if(read_ret == -1)
        {
            printf("Reading error.");
            return EXIT_FAILURE;
        }

    } while(*letter != '\0');

    close(fd_input);
    close(fd_output);

    return EXIT_SUCCESS;
}
