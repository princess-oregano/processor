#include <stdio.h>
#include <assert.h>
#include "text2code.h"
#include "process.h"
#include "stack.h"

static void cpu_dump(int *cmd_buf, int cmd_count, int ip)
{
        printf("CPU dump:\n");

        for (int i = 0; i < cmd_count; i++)
                printf("%02d ", i);

        printf("\n");

        for (int i = 0; i < cmd_count; i++)
                printf("%02d ", cmd_buf[i]);

        printf("\n");

        for (int i = 0; i < ip; i++)
                printf("   ");

        printf("^\n");
}

void 
execute(int *cmd_buf, size_t size)
{
        int val1 = 0;
        int val2 = 0;
        stack_t stack {};
        size_t cmd_count = 0;

        stack_ctor(&stack, DEF_STACK_CAPACITY, VAR_INFO(stack));

        bool quit = false;
        while (cmd_count < size && !quit) {

                int cmd = cmd = cmd_buf[cmd_count];
                switch (cmd) {
                        case CMD_PUSH:
                                cmd_count++;
                                stack_push(&stack, cmd_buf[cmd_count]);
                                cmd_count++;
                                break;
                        case CMD_HLT:
                                cmd_count++;
                                quit = true;
                                break;
                        case CMD_ADD:
                                cmd_count++;
                                stack_pop(&stack, &val1);
                                stack_pop(&stack, &val2);
                                stack_push(&stack, val1 + val2);
                                break;
                        case CMD_SUB:
                                cmd_count++;
                                stack_pop(&stack, &val1);
                                stack_pop(&stack, &val2);
                                stack_push(&stack, val1 - val2);
                                break;
                        case CMD_MUL:
                                cmd_count++;
                                stack_pop(&stack, &val1);
                                stack_pop(&stack, &val2);
                                stack_push(&stack, val1 * val2);
                                break;
                        case CMD_DIV:
                                cmd_count++;
                                stack_pop(&stack, &val1);
                                stack_pop(&stack, &val2);
                                stack_push(&stack, val2 / val1);
                                break;
                        case CMD_OUT:
                                cmd_count++;
                                stack_pop(&stack, &val1);
                                printf("%d\n", val1); 
                                break;
                        case CMD_DMP:
                                cpu_dump(cmd_buf, size, cmd_count);
                                cmd_count++;
                                break;
                        default:
                                cmd_count++;
                                assert(0 && "Invalid command.\n");

                }
                
                cmd = cmd_buf[cmd_count];
        }

        stack_dtor(&stack);
}

