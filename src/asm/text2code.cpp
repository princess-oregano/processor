#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "text2code.h"
#include "../stack.h"

const double THRESHOLD = 1e-10;

static bool 
are_equal(double value1, double value2)
{
        return (fabs(value1 - value2) < THRESHOLD);
}

///////////////////////LABELS///////////////////////

// Finds label in the array.
static int
find_label(label_t *labels, size_t size, char *str)
{
        char label[LABEL_SIZE] = "";
        size_t len = strlen(str);

        if (str[len-1] == ':')
                len--;

        memcpy(label, str, len);
        for (size_t i = 0; i < size; i++) {
                if (strcmp(labels[i].name, label) == 0) {
                        return (int) labels[i].ip;
                }
        }

        return -1;
}

// Creates label.
static void
make_label(label_t *labels, size_t ip, size_t *size, char *str)
{
        size_t len = strlen(str); 
        if (str[len - 1] == ':')
                len--;

        len = (len < LABEL_SIZE) ? len : LABEL_SIZE;

        labels[*size].n_label = *size;
        labels[*size].ip = ip;
        memcpy(labels[*size].name, str, len);

        *size += 1;
}

// Dumps info about labels.
static void
label_dump(label_t *labels, size_t size)
{
        for (size_t i = 0; i < size; i++) {
                fprintf(stderr, "Label %zu:\n"
                                "Name '%s'\n"
                                "IP '%zu'\n", labels[i].n_label,
                                              labels[i].name,
                                              labels[i].ip);

                fprintf(stderr, "\n");
        }
}

///////////////////END_LABELS///////////////////////

void
generate(text_t *text, cmd_arr_t *cmd_arr)
{
        double *cmd_array = (double *) calloc(text->num_of_lines * 2, sizeof(double));

        stack_t ret_ip {};
        stack_ctor(&ret_ip, FUNCS_NUM, VAR_INFO(ret_ip));

        char cmd_name[MAX_CMD_SIZE] = {};
        label_t labels[LABELS_NUM] = {};
        size_t ip = 0;
        size_t  line_count = 0;
        size_t label_count = 0;
        char str_val[40] = {};
        double val = 0;
        char c = 0;

        int cycle_count = 0;
        while (cycle_count < 2 && line_count < text->num_of_lines) {
                sscanf(text->lines[line_count].first_ch, "%s", cmd_name);

                if (strcasecmp(cmd_name, "PUSH") == 0) {
                        IF(PUSH, %lf, &val, (IMMED_MASK), val)
                        IF(PUSH, [%lf], &val, (IMMED_MASK | RAM_MASK), val)
                        IF(PUSH, %s, str_val, (REG_MASK), str_val[1] - 'a' + 1)
                        IF(PUSH, [%s], str_val, (REG_MASK | RAM_MASK), str_val[1] - 'a' + 1)
                        {
                                fprintf(stderr, "Error: invalid PUSH usage.\n");

                                return;
                        }
                } else if (strcasecmp(cmd_name, "POP") == 0) {
                        IF(POP, [%lf], &val, (IMMED_MASK | RAM_MASK), val)
                        IF(POP, %s, str_val, (REG_MASK), str_val[1] - 'a' + 1)
                        IF(POP, [%s], str_val, (REG_MASK | RAM_MASK), str_val[1] - 'a' + 1)
                        {
                                fprintf(stderr, "Error: invalid POP usage.\n");

                                return;
                        }
                } else if (strcasecmp(cmd_name, "JMP") == 0) {
                        cmd_array[ip++] = CMD_JMP;

                        if (sscanf(text->lines[line_count].first_ch +
                        strlen("JMP"), " %c%lf", &c, &val) == 2
                        && c == ':') {
                                cmd_array[ip++] = val;
                        } else if (sscanf(text->lines[line_count].first_ch +
                        strlen("JMP"), " %c%s", &c, str_val) == 2
                        && c == ':') {
                                cmd_array[ip++] =
                                find_label(labels, label_count, str_val);
                        }
                } else if(strcasecmp(cmd_name, "CALL") == 0) {
                        cmd_array[ip++] = CMD_CALL;

                        if (sscanf(text->lines[line_count].first_ch +
                        strlen("CALL"), "%s", str_val) == 1) {
                                if (!are_equal((cmd_array[ip++] =
                                (double) find_label(labels, label_count, str_val)), -1)) {
                                        stack_push(&ret_ip, (double) ip);
                                }
                        }
                } else if(strcasecmp(cmd_name, "RET") == 0) {
                        cmd_array[ip++] = CMD_RET;
                        stack_pop(&ret_ip, &cmd_array[ip++]);
                } else if (strcasecmp(cmd_name, "SQRT") == 0) {
                        cmd_array[ip++] = CMD_SQRT;
                        if (sscanf(text->lines[line_count].first_ch +
                        strlen("SQRT"), "%lf", &val) == 1) {
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
                        if (find_label(labels, label_count, cmd_name) == -1) {
                                if (cycle_count > 0) {
                                        fprintf(stderr, "Error: Couldn't find "
                                                "command '%s'\n", cmd_name);

                                        return;
                                }

                                make_label(labels, ip, &label_count, cmd_name);
                        }
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

        stack_dtor(&ret_ip);
}

void
write_listing(cmd_arr_t cmd_arr)
{
        FILE *list = nullptr;
        list = fopen("listing.txt", "w");

        for (size_t ip = 0; ip < cmd_arr.cmd_count; ) {
                fprintf(list,"%04zu", ip);
                fprintf(list, "    ");
                ip++;
                switch (((int) cmd_arr.cmd_array[ip - 1]) & CMD_MASK) {
                        case CMD_PUSH:
                                fprintf(list, "PUSH");
                                fprintf(list, " %lg", cmd_arr.cmd_array[ip++]);
                                break;
                        case CMD_POP:
                                fprintf(list, "POP");
                                fprintf(list, " %lg", cmd_arr.cmd_array[ip++]);
                                break;
                        case CMD_JMP:
                                fprintf(list, "JMP");
                                fprintf(list, " %lg", cmd_arr.cmd_array[ip++]);
                                break;
                        case CMD_CALL:
                                fprintf(list, "CALL");
                                fprintf(list, " %lg", cmd_arr.cmd_array[ip++]);
                                break;
                        case CMD_RET:
                                fprintf(list, "RET");
                                fprintf(list, " %lg", cmd_arr.cmd_array[ip++]);
                                break;
                        case CMD_HLT:
                                fprintf(list, "HLT");
                                break;
                        case CMD_ADD:
                                fprintf(list, "ADD");
                                break;
                        case CMD_SUB:
                                fprintf(list, "SUB");
                                break;
                        case CMD_MUL:
                                fprintf(list, "MUL");
                                break;
                        case CMD_DIV:
                                fprintf(list, "DIV");
                                break;
                        case CMD_OUT:
                                fprintf(list, "OUT");
                                break;
                        case CMD_DUP:
                                fprintf(list, "DUP");
                                break;
                        case CMD_IN:
                                fprintf(list, "IN");
                                break;
                        case CMD_SQRT:
                                fprintf(list, "SQRT");
                                break;
                        case CMD_DMP & CMD_MASK:
                                fprintf(list, "DMP");
                                break;
                        default:
                                assert(0 && "Invalid usage.");
                                break;
                }
                fprintf(list, "\n");
        }

        fprintf(list, "\n");
}

