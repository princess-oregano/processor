#ifndef TEXT2CODE_H
#define TEXT2CODE_H

#include <stdio.h>
#include "text.h"

const size_t MAX_CMD_SIZE = 5;

enum cmd_t {
        CMD_HLT  = 0,
        CMD_PUSH = 1,
        CMD_ADD  = 2,
        CMD_SUB  = 3,
        CMD_MUL  = 4,
        CMD_DIV  = 5,
        CMD_OUT  = 6,
        CMD_DMP  = 0xBEBEBEBE,
};

// Converts text to int array with machine code.
void
text2code(text_t *text, cmd_arr_t *cmd_arr);

#endif // TEXT2CODE_H

