#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "text.h"

int
main(int argc, char *argv[])
{
        file_t src {};

        get_file(argv[1], &src, "r"); 

        int *cmd_buf = (int *) calloc(src.file_stats.st_size, sizeof(int));

        fread(cmd_buf, sizeof(int), src.file_stats.st_size / sizeof(int),
              src.file_ptr);          

        execute(cmd_buf, src.file_stats.st_size / sizeof(int)); 

        free(cmd_buf);

        return 0;
}

