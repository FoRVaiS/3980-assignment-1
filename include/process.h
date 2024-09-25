#ifndef BENCHIANG_PROCESS_H
#define BENCHIANG_PROCESS_H

#include "filters.h"

int process(int fd_input, int fd_output, const char *filename_input, filter_func filter);

#endif
