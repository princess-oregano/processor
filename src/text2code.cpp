#include <stdlib.h>
#include <string.h>
#include "text2code.h"

void
text2code(text_t *text, cmd_arr_t *cmd_arr)
{
        //fprintf(stderr, "num_of_lines: %zu\n", text->num_of_lines);
        int *cmd_array = (int *) calloc(text->num_of_lines * 2, sizeof(int));

        size_t cmd_count = 0;
        char cmd_name[MAX_CMD_SIZE] = {};
        int val = 0;
        size_t line_count = 0;

        while (line_count < text->num_of_lines) {
                sscanf(text->lines[line_count].first_ch, "%s", cmd_name);

                if (strcmp(cmd_name, "push") == 0) {
                        cmd_array[cmd_count++] = CMD_PUSH;
                        sscanf(text->lines[line_count].first_ch +
                               strlen("push"), "%d", &val);
                        cmd_array[cmd_count++] = val;
                } else if (strcmp(cmd_name, "add") == 0) {
                        cmd_array[cmd_count++] = CMD_ADD;
                } else if (strcmp(cmd_name, "sub") == 0) {
                        cmd_array[cmd_count++] = CMD_SUB;
                } else if (strcmp(cmd_name, "mul") == 0) {
                        cmd_array[cmd_count++] = CMD_MUL;
                } else if (strcmp(cmd_name, "div") == 0) {
                        cmd_array[cmd_count++] = CMD_DIV;
                } else if (strcmp(cmd_name, "out") == 0) {
                        cmd_array[cmd_count++] = CMD_OUT;
                } else if (strcmp(cmd_name, "dmp") == 0) {
                        cmd_array[cmd_count++] = CMD_DMP;
                } else if (strcmp(cmd_name, "jmp") == 0) {
                        cmd_array[cmd_count++] = CMD_JMP;
                        sscanf(text->lines[line_count].first_ch +
                               strlen("jmp"), "%d", &val);
                        cmd_array[cmd_count++] = val;
                } else if (strcmp(cmd_name, "dup") == 0) {
                        cmd_array[cmd_count++] = CMD_DUP;
                } else if (strcmp(cmd_name, "hlt") == 0) {
                        cmd_array[cmd_count++] = CMD_HLT;
                }

                line_count++;
        }

        cmd_arr->cmd_array = cmd_array;
        cmd_arr->cmd_count = cmd_count;
}

