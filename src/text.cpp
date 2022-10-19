#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "text.h"

int
get_file(const char *filename, file_t *file, const char *mode)
{
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

int
read_file(text_t *text, file_t *src)
{
        assert(text);
        assert(src);

        text->buf_size = (size_t) src->file_stats.st_size;

        char *buffer = nullptr;
        if ((buffer = (char *) mmap(NULL, text->buf_size, PROT_READ, MAP_PRIVATE,
                                    fileno(src->file_ptr), 0)) == MAP_FAILED) {

                fprintf(stderr, "Error: Couldn't allocate memory.\n");
                return ERR_ALLOC;
        }

        text->buffer = buffer;

        return ERR_NO_ERR;
}

int
create_lines_arr(text_t *text)
{
        assert(text);

        size_t i = 0;
        size_t line_count = 0;
        line_t *lines_array = nullptr;

        while (true) {
                if (text->buffer[i] == '\n' || text->buffer[i] == '\0')
                        line_count++;

                if (text->buffer[i] == '\0')
                        break;

                i++;
        }

        if ((lines_array = (line_t *) calloc(line_count, sizeof(line_t))) == nullptr) {
                fprintf(stderr, "Error: Couldn't allocate memory for lines_array.\n");
                return ERR_ALLOC;
        }

        int ch = 0;
        size_t index = 0;
        for (index = 0, i = 0; index < line_count && i < text->buf_size; ) {
                while (isspace(ch = text->buffer[i]))
                        i++;

                if (ch != '\n' && ch != '\0') {
                        if (ch == ';') {
                                while (ch != '\n' && ch != '\0') {
                                        i++;
                                        ch = text->buffer[i];
                                }
                                continue;
                        }
                        lines_array[index].first_ch = &text->buffer[i];

                        while (ch != '\n' && ch != '\0') {
                                i++;
                                ch = text->buffer[i];
                        }

                        lines_array[index].last_ch = (&text->buffer[i]);

                        index++;
                }
        }

        text->lines = lines_array;
        text->num_of_lines = index;

        return ERR_NO_ERR;
}

int
write_code(cmd_arr_t cmd_arr, file_t *dst)
{
        setvbuf(dst->file_ptr, NULL, _IONBF, 0);

        fwrite(cmd_arr.cmd_array, sizeof(int), cmd_arr.cmd_count, dst->file_ptr);

        return ERR_NO_ERR;
}

void
destroy_text(text_t *text, cmd_arr_t *cmd_arr, char *dst_name)
{
        assert(text);

        if (text->lines != nullptr)
                free(text->lines);
        if (cmd_arr->cmd_array != nullptr)
                free(cmd_arr->cmd_array);
        if (text->buffer != nullptr)
                munmap(text->buffer, text->buf_size);
        if (dst_name != nullptr)
                free(dst_name);
}

char *
change_ext(char *src_filename, const char *ext)
{
        char *c = strrchr(src_filename, '.');
        size_t c_len = (size_t) (c - src_filename) + strlen(ext);
        char *dst_filename = (char *) calloc (c_len + 1, sizeof(char));

        strncpy(dst_filename, src_filename, c_len - strlen(ext));
        dst_filename[c_len - strlen(ext)] = '\0';
        fprintf(stderr, "dst_filename = %s\n", dst_filename);
        strncat(dst_filename, ext, strlen(ext));

        return dst_filename;
}

