#include "filters.h"

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
