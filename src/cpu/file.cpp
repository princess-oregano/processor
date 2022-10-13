#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../include/text.h"

int
get_file(const char *filename, file_t *file, const char *mode)
{
        mode = "r";

        if ((file->file_ptr = fopen(filename, mode)) == nullptr) {
                fprintf(stderr, "Error: Couldn't open %s.\n", filename);

                return ERR_OPEN_FILE;
        }

        if (stat(filename, &file->file_stats) != 0) {
                fprintf(stderr, "Error: Coudn't get stats of %s.\n", filename);
                return ERR_STATS;
        }

        return ERR_NO_ERR;
}

