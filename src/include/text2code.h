#ifndef TEXT2CODE_H
#define TEXT2CODE_H

#include <stdio.h>
#include "text.h"

#define CMD(name) if (strcasecmp(cmd_name, #name) == 0) { \
                        cmd_array[ip++] = CMD_##name;       \
                  } else   

const size_t MAX_CMD_SIZE = 40;
const int LABEL_SIZE = 40;
const int LABELS_NUM = 50;

const int RAM_MASK = 0x80;
const int REG_MASK = 0x40;
const int IMMED_MASK = 0x20;
const int CMD_MASK = 0x1F;

struct label_t {
        size_t ip = 0;
        size_t n_label = 0;
        char name[LABEL_SIZE] = "";
};

enum cmd_t {
        CMD_HLT  = 0,
        CMD_PUSH = 1,
        CMD_POP  = 2,
        CMD_ADD  = 3,
        CMD_SUB  = 4,
        CMD_MUL  = 5,
        CMD_DIV  = 6,
        CMD_OUT  = 7,
        CMD_JMP  = 8,
        CMD_DUP  = 9,
        CMD_IN   = 10,
        CMD_SQRT = 11,
        CMD_CALL = 12,
        CMD_RET  = 13,
        CMD_DMP  = -1,
};

// Converts text to int array with machine code.
void
text2code(text_t *text, cmd_arr_t *cmd_arr);

#endif // TEXT2CODE_H

