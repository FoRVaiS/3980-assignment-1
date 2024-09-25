#include "filters.h"
#include <ctype.h>     // toupper, tolower
#include <stdio.h>     // printf
#include <string.h>    // strcmp

void filter_upper(char *str)
{
    while((*str) != '\0')
    {
        *str = (char)toupper(*str);
        str++;
    }
}

void filter_lower(char *str)
{
    while((*str) != '\0')
    {
        *str = (char)tolower(*str);
        str++;
    }
}

void filter_none(char *str)
{
    while((*str) != '\0')
    {
        str++;
    }
}

filter_func use_filter(char *filter_name)
{
    if(filter_name == NULL)
    {
        return NULL;
    }

    filter_lower(filter_name);

    if(strcmp(filter_name, "upper") == 0)
    {
        return &filter_upper;
    }

    if(strcmp(filter_name, "lower") == 0)
    {
        return &filter_lower;
    }

    if(strcmp(filter_name, "null") == 0)
    {
        return &filter_none;
    }

    printf("\"%s\" is not a valid filter, please choose one of three valid filters: upper, lower, null\n", filter_name);
    return NULL;
}
