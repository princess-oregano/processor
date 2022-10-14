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

                if (strcasecmp(cmd_name, "PUSH") == 0) {
                        if (sscanf(text->lines[line_count].first_ch +
                            strlen("PUSH"), "%d", &val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("PUSH"), "[%d]", &val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | RAM_MASK | IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("PUSH"), "%s", str_val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | REG_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("PUSH"), "[%s]", str_val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | REG_MASK | RAM_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        }
                } else if (strcasecmp(cmd_name, "POP") == 0) {
                        if (sscanf(text->lines[line_count].first_ch +
                                strlen("POP"), "[%d]", &val) == 1) {
                                cmd_array[ip++] = CMD_POP | RAM_MASK | IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("POP"), "%s", str_val) == 1) {
                                cmd_array[ip++] = CMD_POP | REG_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("POP"), "[%s]", str_val) == 1) {
                                cmd_array[ip++] = CMD_POP | REG_MASK | RAM_MASK;
                                cmd_array[ip++] = str_val[1] - 'a';
                        }
                } else if (strcasecmp(cmd_name, "ADD") == 0) {
                        cmd_array[ip++] = CMD_ADD;
                } else if (strcasecmp(cmd_name, "SUB") == 0) {
                        cmd_array[ip++] = CMD_SUB;
                } else if (strcasecmp(cmd_name, "MUL") == 0) {
                        cmd_array[ip++] = CMD_MUL;
                } else if (strcasecmp(cmd_name, "DIV") == 0) {
                        cmd_array[ip++] = CMD_DIV;
                } else if (strcasecmp(cmd_name, "OUT") == 0) {
                        cmd_array[ip++] = CMD_OUT;
                } else if (strcasecmp(cmd_name, "DMP") == 0) {
                        cmd_array[ip++] = CMD_DMP;
                } else if (strcasecmp(cmd_name, "JMP") == 0) {
                        cmd_array[ip++] = CMD_JMP;
                        sscanf(text->lines[line_count].first_ch +
                               strlen("JMP"), "%d", &val);
                        cmd_array[ip++] = val;
                } else if (strcasecmp(cmd_name, "DUP") == 0) {
                        cmd_array[ip++] = CMD_DUP;
                } else if (strcasecmp(cmd_name, "IN") == 0) {
                        cmd_array[ip++] = CMD_IN;
                } else if (strcasecmp(cmd_name, "HLT") == 0) {
                        cmd_array[ip++] = CMD_HLT;
                } else { 
                        fprintf(stderr, "Error: Couldn't find command '%s'\n", cmd_name);
                }

                line_count++;
        }

        cmd_arr->cmd_array = cmd_array;
        cmd_arr->cmd_count = ip;
}

