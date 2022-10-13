#include <stdio.h>
#include <stdlib.h>
#include "../include/process.h"
#include "../include/text.h"

int
main(int argc, char *argv[])
{
        file_t src {};
        
        get_file(argv[1], &src, "");

        size_t file_size = (size_t) src.file_stats.st_size;
        
        int *cmd_buf = (int *) calloc(file_size, sizeof(int));

        fread(cmd_buf, sizeof(int), file_size / sizeof(int),
              src.file_ptr);

        execute(cmd_buf, file_size / sizeof(int));

        free(cmd_buf);

        return 0;
}

