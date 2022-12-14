#include <stdio.h>
#include <string.h>
#include "../args.h"
#include "../text.h"
#include "text2code.h"

int
main(int argc, char *argv[])
{
        params_t params {};
        process_args(argc, argv, &params);

        for (int i = 1; i < params.files_num; i++) {
                file_t src {};
                file_t dst {};
                text_t text {};
                cmd_arr_t cmd_arr {};

                if ((get_file(params.filename[i].src, &src, "r") == ERR_STATS) ||
                    (get_file(params.filename[i].dst, &dst, "w") == ERR_STATS))
                        return ERR_STATS;

                if (read_file(&text, &src) == ERR_ALLOC)
                        return ERR_ALLOC;

                fclose(src.file_ptr);

                if (create_lines_arr(&text) == ERR_ALLOC)
                        return ERR_ALLOC;

                generate(&text, &cmd_arr);

                write_listing(cmd_arr);

                if (write_code(cmd_arr, &dst) == ERR_ALLOC)
                        return ERR_ALLOC;

                destroy_text(&text, &cmd_arr, params.filename[i].dst);

                fclose(dst.file_ptr);
        }

        return 0;
}

