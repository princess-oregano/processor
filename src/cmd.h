#ifndef CMD_H
#define CMD_H

#define DEF_CMD(cmd, ...) CMD_##cmd,
#define DEF_JMP(cmd, ...) CMD_##cmd,
enum cmd_t {
        CMD_PUSH,
        CMD_POP,
        #include "cmds.inc"
};
#undef DEF_CMD
#undef DEF_JMP

#include <stdio.h>

struct cmd_arr_t {
        double *cmd_array;
        size_t cmd_count;
};

#define DEF_REG(cmd) REG_##cmd,
enum reg_t {
        #include "regs.inc"
};
#undef DEF_REG

const int RAM_MASK   = 0x80;
const int REG_MASK   = 0x40;
const int IMMED_MASK = 0x20;
const int CMD_MASK   = 0x1F;

#endif // CMD_H

