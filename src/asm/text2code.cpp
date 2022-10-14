#include <stdlib.h>
#include <string.h>
#include "../include/text2code.h"

void
text2code(text_t *text, cmd_arr_t *cmd_arr)
{
        //fprintf(stderr, "num_of_lines: %zu\n", text->num_of_lines);
        int *cmd_array = (int *) calloc(text->num_of_lines * 2, sizeof(int));

        size_t ip = 0;
        char cmd_name[MAX_CMD_SIZE] = {};
        char str_val[10] = {};
        int val = 0;
        size_t line_count = 0;

        while (line_count < text->num_of_lines) {
                sscanf(text->lines[line_count].first_ch, "%s", cmd_name);

                if (strcmp(cmd_name, "push") == 0) {
                        if (sscanf(text->lines[line_count].first_ch +
                            strlen("push"), "%d", &val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("push"), "[%d]", &val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | RAM_MASK | IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("push"), "%s", str_val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | REG_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("push"), "[%s]", str_val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | REG_MASK | RAM_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        }
                } else if (strcmp(cmd_name, "pop") == 0) {
                        if (sscanf(text->lines[line_count].first_ch +
                                strlen("pop"), "[%d]", &val) == 1) {
                                cmd_array[ip++] = CMD_POP | RAM_MASK | IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("pop"), "%s", str_val) == 1) {
                                cmd_array[ip++] = CMD_POP | REG_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("pop"), "[%s]", str_val) == 1) {
                                cmd_array[ip++] = CMD_POP | REG_MASK | RAM_MASK;
                                cmd_array[ip++] = str_val[1] - 'a';
                        }
                } else if (strcmp(cmd_name, "add") == 0) {
                        cmd_array[ip++] = CMD_ADD;
                } else if (strcmp(cmd_name, "sub") == 0) {
                        cmd_array[ip++] = CMD_SUB;
                } else if (strcmp(cmd_name, "mul") == 0) {
                        cmd_array[ip++] = CMD_MUL;
                } else if (strcmp(cmd_name, "div") == 0) {
                        cmd_array[ip++] = CMD_DIV;
                } else if (strcmp(cmd_name, "out") == 0) {
                        cmd_array[ip++] = CMD_OUT;
                } else if (strcmp(cmd_name, "dmp") == 0) {
                        cmd_array[ip++] = CMD_DMP;
                } else if (strcmp(cmd_name, "jmp") == 0) {
                        cmd_array[ip++] = CMD_JMP;
                        sscanf(text->lines[line_count].first_ch +
                               strlen("jmp"), "%d", &val);
                        cmd_array[ip++] = val;
                } else if (strcmp(cmd_name, "dup") == 0) {
                        cmd_array[ip++] = CMD_DUP;
                } else if (strcmp(cmd_name, "in") == 0) {
                        cmd_array[ip++] = CMD_IN;
                } else if (strcmp(cmd_name, "hlt") == 0) {
                        cmd_array[ip++] = CMD_HLT;
                } else { 
                        fprintf(stderr, "Error: Couldn't find command '%s'\n", cmd_name);
                }

                line_count++;
        }

        cmd_arr->cmd_array = cmd_array;
        cmd_arr->cmd_count = ip;
}

