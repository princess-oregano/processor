#include <stdio.h>
#include "../include/error.h"

void
decypher_error_stack(err_u err)
{
        if (err.val == 0)
                return;

        if (err.type.ERR_ALLOC) {
                fprintf(stderr, "Couldn't allocate memory.\n");
        }

        if (err.type.ERR_BAD_SIZE) {
                fprintf(stderr, "Invalid size.\n");
        }

        if (err.type.ERR_ALLOC) {
                fprintf(stderr, "Invalid capacity.\n");
        }
}

