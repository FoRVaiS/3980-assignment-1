#ifndef BENCHIANG_FILTERS_H
#define BENCHIANG_FILTERS_H

typedef void (*filter_func)(char *str);

void filter_upper(char *str);
void filter_lower(char *str);
void filter_none(char *str);

filter_func use_filter(char *filter_name);

#endif
