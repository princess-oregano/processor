#include <stdio.h>
#include "text.h"
#include "text2code.h"

int
main(int argc, char *argv[])
{
        file_t src {};
        file_t dst {};
        text_t text {};
        cmd_arr_t cmd_arr {};

        if ((get_file(argv[1], &src, "r") == ERR_STATS) ||
            (get_file("code.mur", &dst, "w") == ERR_STATS))
                return ERR_STATS;

        if (read_file(&text, &src) == ERR_ALLOC)
                return ERR_ALLOC;

        fclose(src.file_ptr);

        if (create_lines_arr(&text) == ERR_ALLOC)
                return ERR_ALLOC;

        text2code(&text, &cmd_arr);

        if (write_code(cmd_arr, &dst) == ERR_ALLOC)
                return ERR_ALLOC;

        destroy_text(&text, &cmd_arr);

        fclose(dst.file_ptr);

        return 0;
}

