#include <stdlib.h>
#include <string.h>
#include "../include/text2code.h"

void
text2code(text_t *text, cmd_arr_t *cmd_arr)
{
        int *cmd_array = (int *) calloc(text->num_of_lines * 2, sizeof(int));

        size_t ip = 0;
        char cmd_name[MAX_CMD_SIZE] = {};
        label_t labels[LABELS_NUM] = {};
        size_t label_count = 0;
        char str_val[40] = {};
        int val = 0;
        size_t line_count = 0;
       
        int cycle_count = 0;
        while (cycle_count < 2) {
        line_count = 0;
        ip = 0;
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
                        if (sscanf(text->lines[line_count].first_ch +
                        strlen("JMP"), "%d", &val) == 1) {
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                        strlen("JMP"), "%s", str_val) == 1) {
                                fprintf(stderr, "%s\n", str_val);
                                cmd_array[ip] = -1;
                                for (size_t i = 0; i < label_count; i++) {
                                        if (strcasecmp(labels[i].name, str_val) == 0) {
                                                cmd_array[ip++] = (int) labels[i].ip;
                                        }
                                }
                        }
                } else if (strcasecmp(cmd_name, "DUP") == 0) {
                        cmd_array[ip++] = CMD_DUP;
                } else if (strcasecmp(cmd_name, "IN") == 0) {
                        cmd_array[ip++] = CMD_IN;
                } else if (strcasecmp(cmd_name, "HLT") == 0) {
                        cmd_array[ip++] = CMD_HLT;
                } else { 
                        fprintf(stderr, "Error: Couldn't find command '%s'\n", cmd_name);
                        labels[label_count].n_label = label_count;
                        labels[label_count].ip = ip;
                        fprintf(stderr, "%zu\n", labels[label_count].ip); 
                        strcpy(labels[label_count++].name, cmd_name);
                }

                line_count++;
        }
        cycle_count++;
        }

        cmd_arr->cmd_array = cmd_array;
        cmd_arr->cmd_count = ip;
}

