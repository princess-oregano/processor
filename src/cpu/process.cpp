#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "../include/text2code.h"
#include "../include/process.h"
#include "../include/stack.h"

static void cpu_dump(int *cmd_buf, size_t cmd_count, size_t ip)
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

        bool quit = false;
        while (ip < size && !quit) {
                sleep(1);
                int cmd = cmd_buf[ip];
                switch (cmd & CMD_MASK) {
                        case CMD_PUSH:
                                ip++;
                                if ((cmd & (RAM_MASK | IMMED_MASK)) == (RAM_MASK | IMMED_MASK))
                                        stack_push(&stack, ram[cmd_buf[ip]]);
                                else if ((cmd & (REG_MASK | RAM_MASK)) == (REG_MASK | RAM_MASK))
                                        stack_push(&stack, ram[reg[ip]]);
                                else if ((cmd & IMMED_MASK) == IMMED_MASK)
                                        stack_push(&stack, cmd_buf[ip]);
                                else if ((cmd & REG_MASK) == REG_MASK)
                                        stack_push(&stack, reg[cmd_buf[ip]]);
                                ip++;
                                break;
                        case CMD_POP:
                                ip++;
                                if ((cmd & (RAM_MASK | IMMED_MASK)) == (RAM_MASK | IMMED_MASK))
                                        stack_pop(&stack, &ram[cmd_buf[ip]]);
                                else if ((cmd & (REG_MASK | RAM_MASK)) == (REG_MASK | RAM_MASK))
                                        stack_pop(&stack, &ram[reg[ip]]);
                                else if ((cmd & REG_MASK) == REG_MASK)
                                        stack_pop(&stack, &reg[cmd_buf[ip]]);
                                ip++;
                                break;
                        case CMD_HLT:
                                ip++;
                                quit = true;
                                break;
                        case CMD_ADD:
                                ip++;
                                stack_pop(&stack, &val1);
                                stack_pop(&stack, &val2);
                                stack_push(&stack, val1 + val2);
                                break;
                        case CMD_SUB:
                                ip++;
                                stack_pop(&stack, &val1);
                                stack_pop(&stack, &val2);
                                stack_push(&stack, val1 - val2);
                                break;
                        case CMD_MUL:
                                ip++;
                                stack_pop(&stack, &val1);
                                stack_pop(&stack, &val2);
                                stack_push(&stack, val1 * val2);
                                break;
                        case CMD_DIV:
                                ip++;
                                stack_pop(&stack, &val1);
                                stack_pop(&stack, &val2);
                                stack_push(&stack, val2 / val1);
                                break;
                        case CMD_OUT:
                                ip++;
                                stack_pop(&stack, &val1);
                                printf("%d\n", val1);
                                break;
                        case (CMD_DMP & CMD_MASK):
                                cpu_dump(cmd_buf, size, ip);
                                ip++;
                                break;
                        case CMD_JMP:
                                ip++;
                                ip = cmd_buf[ip];
                                break;
                        case CMD_DUP:
                                stack_pop(&stack, &val1);
                                stack_push(&stack, val1);
                                stack_push(&stack, val1);
                                ip++;
                                break;
                        case CMD_IN:
                                scanf("%d", &val1);
                                stack_push(&stack, val1);
                                break;
                        default:
                                ip++;
                                assert(0 && "Invalid command.\n");

                }

                cmd = cmd_buf[ip];
        }

        stack_dtor(&stack);
}

