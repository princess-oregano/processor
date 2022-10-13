#include <stdio.h>
#include <string.h>
#include "../include/text.h"
#include "../include/text2code.h"

const char *file_extension = ".mur";

int
main(int argc, char *argv[])
{
        size_t num_of_files = (size_t) argc;

        for (size_t file_count = 1; file_count < num_of_files; file_count++) {
                file_t src {};
                file_t dst {};
                text_t text {};
                cmd_arr_t cmd_arr {};

                char *c = strrchr(argv[file_count], '.');
                size_t c_len = (size_t) (c - argv[file_count]) + strlen(file_extension);
                char dst_filename[c_len + 1];

                strncpy(dst_filename, argv[file_count], c_len - strlen(file_extension));
                dst_filename[c_len - strlen(file_extension)] = '\0';
                fprintf(stderr, "dst_filename = %s\n", dst_filename);
                strncat(dst_filename, file_extension, strlen(file_extension));

                if ((get_file(argv[file_count], &src, "r") == ERR_STATS) ||
                    (get_file(dst_filename, &dst, "w") == ERR_STATS))
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
        }

        return 0;
}

