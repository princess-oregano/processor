#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"

// Changes file extention.
static char *
change_ext(char *src_filename, const char *ext)
{
        char *c = strrchr(src_filename, '.');
        size_t c_len = (size_t) (c - src_filename) + strlen(ext);
        char *dst_filename = (char *) calloc (c_len + 1, sizeof(char));

        strncpy(dst_filename, src_filename, c_len - strlen(ext));
        dst_filename[c_len - strlen(ext)] = '\0';
        strcat(dst_filename, ext);

        return dst_filename;
}

void
process_args(int argc, char *argv[], params_t *params)
{
        for (int i = 1; i < argc; i++) {
                params->filename[i].src = argv[i];
                params->filename[i].dst = 
                        change_ext(params->filename[i].src, FILE_EXT);
        }

        params->files_num = argc;
}

