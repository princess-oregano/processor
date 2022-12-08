#ifndef TEXT2CODE_H
#define TEXT2CODE_H

#include <stdio.h>
#include "../text.h"
#include "../cmd.h"

#define CMD(name) if (strcasecmp(cmd_name, #name) == 0) { \
                        cmd_array[ip++] = CMD_##name;       \
                  } else
#define IF(cmd, spec, val, masks, adr)           \
if (sscanf(text->lines[line_count].first_ch +    \
strlen(#cmd), #spec, val) == 1) {                \
        cmd_array[ip++] = CMD_##cmd |(masks);    \
        cmd_array[ip++] = (adr);                 \
} else
#define JUMP(cmd) } else if (strcasecmp(cmd_name, #cmd) == 0) { \
                        cmd_array[ip++] = CMD_##cmd;               \
                        if (sscanf(text->lines[line_count].first_ch + \
                        strlen(#cmd), " %c%lf", &c, &val) == 2 \
                        && c == ':') {  \
                                cmd_array[ip++] = val; \
                        } else if (sscanf(text->lines[line_count].first_ch + \
                        strlen(#cmd), " %c%s", &c, str_val) == 2 \
                        && c == ':') { \
                                cmd_array[ip++] = \
                                find_label(labels, label_count, str_val); \
                        }

const size_t MAX_CMD_SIZE = 40;
const int LABELS_NUM = 50;
const int LABEL_SIZE = 40;
const int  FUNCS_NUM = 50;
const int  FUNC_SIZE = 40;

struct label_t {
        size_t ip = 0;
        size_t n_label = 0;
        char name[LABEL_SIZE] = "";
};

struct func_t {
        size_t ip = 0;
        size_t n_func = 0;
        char name[FUNC_SIZE] = "";
};

// Converts text to int array with machine code.
void
generate(text_t *text, cmd_arr_t *cmd_arr);
// Writes listing of program to listing file.
void
write_listing(cmd_arr_t cmd_arr);

#endif // TEXT2CODE_H

