#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "../args.h"
#include "../text.h"

int
main(int argc, char *argv[])
{
        params_t params {};
        process_args(argc, argv, &params);

        for (int i = 1; i < argc; i++) {
                file_t src {};

                get_file(params.filename[i].dst, &src, "r");

                size_t file_size = (size_t) src.file_stats.st_size;

                double *cmd_buf = (double *) calloc(file_size, sizeof(double));

                fread(cmd_buf, sizeof(double), file_size / sizeof(double),
                      src.file_ptr);

                execute(cmd_buf, file_size / sizeof(double));

                free(cmd_buf);
                free(params.filename[i].dst);
        }

        return 0;
}

