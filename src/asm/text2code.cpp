#include <stdlib.h>
#include <string.h>
#include "../include/text2code.h"

// Finds label in the array.
static int
find_label(label_t *labels, size_t size, char *str)
{
        for (size_t i = 0; i < size; i++) {
                if (strcasecmp(labels[i].name, str) == 0) {
                        return (int) labels[i].ip;
                }
        }

        return -1;
}

// Creates label.
static void
make_label(label_t *labels, size_t ip, size_t *size, char *str)
{
        labels[*size].n_label = *size;
        labels[*size].ip = ip;
        strcpy(labels[*size].name, str);

        *size += 1;
}

void
text2code(text_t *text, cmd_arr_t *cmd_arr)
{
        int *cmd_array = (int *) calloc(text->num_of_lines * 2, sizeof(int));

        char cmd_name[MAX_CMD_SIZE] = {};
        label_t labels[LABELS_NUM] = {};
        size_t ip = 0;
        size_t label_count = 0;
        size_t  line_count = 0;
        char str_val[40] = {};
        int val = 0;

        int cycle_count = 0;
        while (cycle_count < 2 && line_count < text->num_of_lines) {
                sscanf(text->lines[line_count].first_ch, "%s", cmd_name);

                if (strcasecmp(cmd_name, "PUSH") == 0) {
                        if (sscanf(text->lines[line_count].first_ch +
                            strlen("PUSH"), "%d", &val) == 1) {
                                cmd_array[ip++] =   CMD_PUSH | 
                                                  IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("PUSH"), "[%d]", &val) == 1) {
                                cmd_array[ip++] =   CMD_PUSH | 
                                                    RAM_MASK | 
                                                  IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("PUSH"), "%s", str_val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | 
                                                  REG_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("PUSH"), "[%s]", str_val) == 1) {
                                cmd_array[ip++] = CMD_PUSH | 
                                                  REG_MASK | 
                                                  RAM_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        }
                } else if (strcasecmp(cmd_name, "POP") == 0) {
                        if (sscanf(text->lines[line_count].first_ch +
                                strlen("POP"), "[%d]", &val) == 1) {
                                cmd_array[ip++] =    CMD_POP | 
                                                    RAM_MASK | 
                                                  IMMED_MASK;
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("POP"), "%s", str_val) == 1) {
                                cmd_array[ip++] =  CMD_POP | 
                                                  REG_MASK;
                                cmd_array[ip++] = str_val[1] - 'a' + 1;
                        } else if (sscanf(text->lines[line_count].first_ch +
                                strlen("POP"), "[%s]", str_val) == 1) {
                                cmd_array[ip++] =  CMD_POP | 
                                                  REG_MASK | 
                                                  RAM_MASK;
                                cmd_array[ip++] = str_val[1] - 'a';
                        }
                } else if (strcasecmp(cmd_name, "JMP") == 0) {
                        cmd_array[ip++] = CMD_JMP;

                        if (sscanf(text->lines[line_count].first_ch +
                        strlen("JMP"), "%d", &val) == 1) {
                                fprintf(stderr, "val = %d\n", val);
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                        strlen("JMP"), "%s", str_val) == 1) {
                                cmd_array[ip++] = 
                                find_label(labels, label_count, str_val);

                        }
                } else if (strcasecmp(cmd_name, "SQRT") == 0) {
                        cmd_array[ip++] = CMD_SQRT;
                        if (sscanf(text->lines[line_count].first_ch +
                        strlen("SQRT"), "%d", &val) == 1) {
                                cmd_array[ip++] = val;
                        }
                } else 
                CMD(ADD) 
                CMD(SUB) 
                CMD(MUL)
                CMD(DIV)
                CMD(OUT)
                CMD(DMP)
                CMD(DUP)
                CMD(IN)
                CMD(HLT) 
                { 
                        if (cycle_count > 0 && 
                            find_label(labels, label_count, cmd_name) == -1) {
                                fprintf(stderr, "Error: Couldn't find "
                                        "command '%s'\n", cmd_name);

                                return;
                        }
                        
                        make_label(labels, ip, &label_count, cmd_name);
                }

                line_count++;
                if (line_count == text->num_of_lines && cycle_count < 1) {
                        cycle_count++;
                        line_count = 0;
                        ip = 0;
                }
        }

        cmd_arr->cmd_array = cmd_array;
        cmd_arr->cmd_count = ip;
}

