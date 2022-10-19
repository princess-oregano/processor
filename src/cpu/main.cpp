#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "../text.h"

int
main(int argc, char *argv[])
{
        size_t num_of_files = (size_t) argc;

        for (size_t file_count = 1; file_count < num_of_files; file_count++) {
                file_t src {};

                get_file(argv[file_count], &src, "r");

                size_t file_size = (size_t) src.file_stats.st_size;

                int *cmd_buf = (int *) calloc(file_size, sizeof(int));

                fread(cmd_buf, sizeof(int), file_size / sizeof(int),
                      src.file_ptr);

                execute(cmd_buf, file_size / sizeof(int));

                free(cmd_buf);
        }

        return 0;
}

