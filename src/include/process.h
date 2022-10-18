#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

#define POP(val) stack_pop(&stack, &val);
#define PUSH(val) stack_push(&stack, val);
#define IF_PUSH(msk, dst)  if ((cmd & (msk)) == (msk)) { \
                                PUSH(dst)                \
                                ip++;                    \
                                }                        \
                        else 
#define IF_POP(msk, dst)  if ((cmd & (msk)) == (msk)) {  \
                                POP(dst)                 \
                                ip++;                    \
                                }                        \
                        else 
#define DEF_CMD(name, ...) case CMD_##name: ip++; __VA_ARGS__ break;

const int N_REG = 5;
const int N_RAM = 15;

void
execute(int *cmd_buf, size_t size);

#endif // PROCESS_H

