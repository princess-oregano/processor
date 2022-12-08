#ifndef CMD_H
#define CMD_H

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
        CMD_JA   = 14,
        CMD_JB   = 15,
        CMD_JE   = 16, 
        CMD_JAE  = 17,
        CMD_JBE  = 18,
        CMD_JNE  = 19,
        CMD_DMP  = -1,
};

#include <stdio.h>
struct cmd_arr_t {
        double *cmd_array;
        size_t cmd_count;
};

enum reg_t {
        REG_RSP = 0,
        REG_RAX = 1,
        REG_RBX = 2,
        REG_RCX = 3,
        REG_RDX = 4,
        REG_REX = 5,
        REG_RFX = 6,
        REG_RGX = 7,
        REG_RHX = 8,
        REG_RIX = 9,
        REG_RJX = 10,
        REG_RKX = 11,
        REG_RLX = 12,
        REG_RMX = 13,
        REG_RNX = 14,
        REG_ROX = 15,
        REG_RPX = 16,
};

const int RAM_MASK   = 0x80;
const int REG_MASK   = 0x40;
const int IMMED_MASK = 0x20;
const int CMD_MASK   = 0x1F;

#endif // CMD_H

