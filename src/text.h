#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <sys/stat.h>
#include "cmd.h"

enum error_text_t {
        ERR_NO_ERR    = 0,
        ERR_OPEN_FILE = 1,
        ERR_STATS     = 2,
        ERR_ALLOC     = 3,
};

struct file_t {
        FILE  *file_ptr = nullptr;
        struct stat file_stats = {};
};

struct line_t {
        char *first_ch = nullptr;
        char  *last_ch = nullptr;
};

struct text_t {
        char *buffer        = nullptr;
        size_t buf_size     = 0;
        line_t *lines       = {};
        size_t num_of_lines = 0;
};

// Opens file and gets info about it.
int
get_file(const char *filename, file_t *file, const char *mode);
// Creates and initializes buffer for storing text.
int
read_file(text_t *text, file_t *src);
// Creates and initializes an array of line_t values.
int
create_lines_arr(text_t *text);
// Frees allocated with malloc()/calloc()/realloc()/etc. space.
void
destroy_text(text_t *text, cmd_arr_t *cmd_arr, char *dst_name);
// Prints given text in stream.
int
write_code(cmd_arr_t cmd_arr, file_t *dst);

#endif // TEXT_H

