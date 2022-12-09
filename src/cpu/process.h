#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

#define POP(val) stack_pop(&stack, &val); reg[REG_RSP] = (double) stack.size; 
#define PUSH(val) stack_push(&stack, val); reg[REG_RSP] = (double) stack.size;
#define IF_PUSH(msk, dst) if ((cmd & (msk)) == (msk)) {  \
                                PUSH(dst)                \
                                ip++;                    \
                                }                        \
                        else
#define IF_POP(msk, dst, ...) if ((cmd & (msk)) == (msk)) {  \
                                POP(dst)                 \
                                __VA_ARGS__              \
                                ip++;                    \
                                }                        \
                        else
#define DEF_CMD(name, ...) case CMD_##name: ip++; __VA_ARGS__ break;

const int N_REG = 16;
const int N_RAM = 300;
// Resolution of screen.
const int RESOL_X = 20;
const int RESOL_Y = 10;

void
execute(double *cmd_buf, size_t size);

#endif // PROCESS_H

