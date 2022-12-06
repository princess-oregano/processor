#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include "../cmd.h"
#include "process.h"
#include "../stack.h"

static void
cpu_dump(int *cmd_buf, size_t cmd_count, size_t ip)
{
        printf("CPU dump:\n");

        for (size_t i = 0; i < cmd_count; i++)
                printf("%02zu ", i);
        printf("\n");

        for (size_t i = 0; i < cmd_count; i++)
                printf("%02d ", cmd_buf[i]);
        printf("\n");

        for (size_t i = 0; i < ip; i++)
                printf("   ");
        printf("^\n");
}

void
execute(int *cmd_buf, size_t size)
{
        int val1 = 0;
        int val2 = 0;
        stack_t stack {};
        size_t ip = 0;
        
        int reg[N_REG] = {};
        int ram[N_RAM] = {};

        stack_ctor(&stack, DEF_STACK_CAPACITY, VAR_INFO(stack));

        bool halt = false;
        while (ip < size && !halt) {
                sleep(1);
                int cmd = cmd_buf[ip];
                switch (cmd & CMD_MASK) {
                        DEF_CMD(PUSH, 
                                IF_PUSH(RAM_MASK | IMMED_MASK, ram[cmd_buf[ip]])
                                IF_PUSH(REG_MASK | RAM_MASK, ram[reg[ip]])
                                IF_PUSH(IMMED_MASK, cmd_buf[ip])
                                IF_PUSH(REG_MASK, reg[cmd_buf[ip]])
                                        assert(0 && "Invalid PUSH command.\n");
                               )
                        DEF_CMD(POP,
                                IF_POP((RAM_MASK | IMMED_MASK), ram[cmd_buf[ip]])
                                IF_POP((REG_MASK | RAM_MASK), ram[reg[ip]])
                                IF_POP((REG_MASK), reg[cmd_buf[ip]])
                                        assert(0 && "Invalid POP command.\n");
                               )
                        DEF_CMD(HLT, halt = true;)
                        DEF_CMD(ADD, POP(val1) POP(val2) PUSH(val1 + val2))
                        DEF_CMD(SUB, POP(val1) POP(val2) PUSH(val1 - val2))
                        DEF_CMD(MUL, POP(val1) POP(val2) PUSH(val1 * val2))
                        DEF_CMD(DIV, POP(val1) POP(val2) PUSH(val2 / val1))
                        DEF_CMD(DUP, POP(val1) PUSH(val1) PUSH(val2))
                        DEF_CMD(OUT, POP(val1) printf("%d\n", val1);)
                        DEF_CMD(JMP, ip = (size_t) cmd_buf[ip];)
                        DEF_CMD(CALL, ip = (size_t) cmd_buf[ip];)
                        DEF_CMD(RET, ip = (size_t) cmd_buf[ip];)
                        DEF_CMD(IN, scanf("%d", &val1); PUSH(val1))
                        DEF_CMD(SQRT, val1 = (int) sqrt(val1); PUSH(val1))
                        DEF_CMD(DMP & CMD_MASK, cpu_dump(cmd_buf, size, ip);)
                        default:
                                ip++;
                                assert(0 && "Invalid command.\n");

                }

                cmd = cmd_buf[ip];
        }

        stack_dtor(&stack);
}

