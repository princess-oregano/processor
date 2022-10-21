#ifndef ARGS_H
#define ARGS_H

#include "text.h"

const int MAX_FILES_NUM = 50;
const char * const FILE_EXT = ".mur";

struct filename_t {
        char *src = nullptr;
        char *dst = nullptr;
};

struct params_t {
        filename_t filename[MAX_FILES_NUM];
        int files_num = 0;
};

// Processes command line arguments.
void
process_args(int argc, char *argv[], params_t *params);

#endif // ARGS_H

