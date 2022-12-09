#ifndef CMD_H
#define CMD_H

enum cmd_t {
        CMD_HLT  = 0,  // Halt the program.
        CMD_PUSH = 1,  // Push value to stack.
        CMD_POP  = 2,  // Pop value from stack.
        CMD_ADD  = 3,  // Add values.
        CMD_SUB  = 4,  // Substract values.
        CMD_MUL  = 5,  // Multiplicate values.
        CMD_DIV  = 6,  // Divide values.
        CMD_OUT  = 7,  // Pop value from stack and print it.
        CMD_JMP  = 8,  // Jump to label.
        CMD_DUP  = 9,  // Duplicate the upper value in stack.
        CMD_IN   = 10, // Scan value from stdin, push it to stack.
        CMD_SQRT = 11, // Pop value from a stack, push it's square root.
        CMD_CALL = 12, // Call function.
        CMD_RET  = 13, // Return to a function.
        CMD_JA   = 14, // Conditinal jumps.
        CMD_JB   = 15, // 
        CMD_JE   = 16, //  
        CMD_JAE  = 17, // 
        CMD_JBE  = 18, // 
        CMD_JNE  = 19, // 
        CMD_SIN  = 20, // Take sin.
        CMD_COS  = 21, // Take cos.
        CMD_PON  = 22, // Set pixel on.
        CMD_PIC  = 23, // Print picture(taken from VRAM).
        CMD_CLN  = 24, // Clean screen.
        CMD_DMP  = -1, // Dump processor.
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

