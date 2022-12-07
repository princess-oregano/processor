#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include "../cmd.h"
#include "process.h"
#include "../stack.h"

static void
cpu_dump(double *cmd_buf, size_t cmd_count, size_t ip)
{
        printf("CPU dump:\n");

        for (size_t i = 0; i < cmd_count; i++)
                printf("%02zu ", i);
        printf("\n");

        for (size_t i = 0; i < cmd_count; i++)
                printf("%02lg ", cmd_buf[i]);
        printf("\n");

        for (size_t i = 0; i < ip; i++)
                printf("   ");
        printf("^\n");
}

void
execute(double *cmd_buf, size_t size)
{
        double val1 = 0;
        double val2 = 0;
        stack_t stack {};
        size_t ip = 0;
        
        double reg[N_REG] = {};
        double ram[N_RAM] = {};

        stack_ctor(&stack, DEF_STACK_CAPACITY, VAR_INFO(stack));

        bool halt = false;
        while (ip < size && !halt) {
                int cmd = (int) cmd_buf[ip];
                switch (cmd & CMD_MASK) {
                        DEF_CMD(PUSH, 
                                IF_PUSH(RAM_MASK | IMMED_MASK, ram[(int) cmd_buf[ip]])
                                IF_PUSH(REG_MASK | RAM_MASK, ram[(int) reg[ip]])
                                IF_PUSH(IMMED_MASK, cmd_buf[ip])
                                IF_PUSH(REG_MASK, reg[(int) cmd_buf[ip]])
                                {
                                assert(0 && "Invalid PUSH command.\n");
                                }
                               )
                        DEF_CMD(POP,
                                IF_POP((RAM_MASK | IMMED_MASK), ram[(int) cmd_buf[ip]], )
                                IF_POP((REG_MASK | RAM_MASK), ram[(int) reg[ip]], )
                                IF_POP((REG_MASK), val1,
                                        reg[(int) cmd_buf[ip]] = val1;
                                        if ((int) cmd_buf[ip] == REG_RSP) {
                                        stack.size = (size_t) reg[REG_RSP];
                                        })
                                {
                                assert(0 && "Invalid POP command.\n");
                                }
                               )
                        DEF_CMD(HLT, halt = true;)
                        DEF_CMD(ADD, POP(val1) POP(val2) PUSH(val1 + val2))
                        DEF_CMD(SUB, POP(val1) POP(val2) PUSH(val1 - val2))
                        DEF_CMD(MUL, POP(val1) POP(val2) PUSH(val1 * val2))
                        DEF_CMD(DIV, POP(val1) POP(val2) PUSH(val2 / val1))
                        DEF_CMD(DUP, POP(val1) PUSH(val1) PUSH(val2))
                        DEF_CMD(OUT, POP(val1) printf("%lg\n", val1);)
                        DEF_CMD(JMP, ip = (size_t) cmd_buf[ip];)
                        DEF_CMD(CALL, PUSH((double) ip+1) ip = (size_t) cmd_buf[ip];)
                        DEF_CMD(RET, POP(val1) ip = (size_t) val1;)
                        DEF_CMD(IN, scanf("%lf", &val1); PUSH(val1))
                        DEF_CMD(SQRT, val1 = sqrt(val1); PUSH(val1))
                        DEF_CMD(DMP & CMD_MASK, cpu_dump(cmd_buf, size, ip);)
                        default:
                                assert(0 && "Invalid command.\n");

                }

                /*
                 *fprintf(stderr, "rsp = %lg\n", reg[REG_RSP]); 
                 *fprintf(stderr, "stack.size = %zu\n", stack.size); 
                 */
                 
                cmd = (int) cmd_buf[ip];
        }

        stack_dtor(&stack);
}

