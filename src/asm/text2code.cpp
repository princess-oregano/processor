#include <stdlib.h>
#include <string.h>
#include "text2code.h"
#include "../stack.h"

//////////////////////FUNCTIONS/////////////////////

// Finds function int the array, returns ip.
static int
find_func(func_t *funcs, size_t size, char *str)
{
        char func[FUNC_SIZE] = "";
        size_t len = strlen(str);

        memcpy(func, str, len);
        for (size_t i = 0; i < size; i++) {
                if (strcmp(funcs[i].name, func) == 0) {
                        return (int) funcs[i].ip;
                }
        }

        return -1;
}

// Makes function.
static void
make_func(func_t *funcs, size_t ip, size_t *size, char *str)
{
        funcs[*size].n_func = *size;
        funcs[*size].ip = ip;
        memcpy(funcs[*size].name, str, strlen(str));

        *size += 1;
}

// Dumps info about functions.
static void
func_dump(func_t *funcs, size_t size)
{
        for (size_t i = 0; i < size; i++) {
                fprintf(stderr, "Function %zu:\n"
                                "Name '%s'\n"
                                "IP '%zu'\n", funcs[i].n_func,
                                              funcs[i].name,
                                              funcs[i].ip);

                fprintf(stderr, "\n");
        }
}

////////////////////END_FUNCTIONS///////////////////

///////////////////////LABELS///////////////////////

// Finds label in the array.
static int
find_label(label_t *labels, size_t size, char *str)
{
        char label[LABEL_SIZE] = "";
        size_t len = strlen(str);

        if (str[len-1] == ':')
                len--;

        strncpy(label, str, len);
        for (size_t i = 0; i < size; i++) {
                if (strcmp(labels[i].name, label) == 0) {
                        return (int) labels[i].ip;
                }
        }

        return -1;
}

// Creates label.
static bool
make_label(label_t *labels, size_t ip, size_t *size, char *str)
{
        size_t len = strlen(str) - 1;

        if (str[len] != ':')
                return false;

        len = (len < LABEL_SIZE) ? len : LABEL_SIZE;

        labels[*size].n_label = *size;
        labels[*size].ip = ip;
        memcpy(labels[*size].name, str, len);

        *size += 1;

        return true;
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
text2code(text_t *text, cmd_arr_t *cmd_arr)
{
        int *cmd_array = (int *) calloc(text->num_of_lines * 2, sizeof(int));

        stack_t ret_ip {};
        stack_ctor(&ret_ip, FUNCS_NUM, VAR_INFO(ret_ip));

        char cmd_name[MAX_CMD_SIZE] = {};
        label_t labels[LABELS_NUM] = {};
        func_t  funcs [FUNCS_NUM]  = {};
        size_t ip = 0;
        size_t  line_count = 0;
        size_t  func_count = 0;
        size_t label_count = 0;
        char str_val[40] = {};
        int val = 0;
        char c = 0;

        int cycle_count = 0;
        while (cycle_count < 2 && line_count < text->num_of_lines) {
                sscanf(text->lines[line_count].first_ch, "%s", cmd_name);

                if (strcasecmp(cmd_name, "PUSH") == 0) {
                        IF(PUSH, %d, &val, (IMMED_MASK), val)
                        IF(PUSH, [%d], &val, (IMMED_MASK | RAM_MASK), val)
                        IF(PUSH, %s, str_val, (REG_MASK), str_val[1] - 'a' + 1)
                        IF(PUSH, [%s], str_val, (REG_MASK | RAM_MASK), str_val[1] - 'a' + 1)
                        {
                                fprintf(stderr, "Error: invalid PUSH usage.\n");

                                return;
                        }
                } else if (strcasecmp(cmd_name, "POP") == 0) {
                        IF(POP, [%d], &val, (IMMED_MASK | RAM_MASK), val)
                        IF(POP, %s, str_val, (REG_MASK), str_val[1] - 'a' + 1)
                        IF(POP, [%s], str_val, (REG_MASK | RAM_MASK), str_val[1] - 'a' + 1)
                        {
                                fprintf(stderr, "Error: invalid POP usage.\n");

                                return;
                        }
                } else if (strcasecmp(cmd_name, "JMP") == 0) {
                        cmd_array[ip++] = CMD_JMP;

                        if (sscanf(text->lines[line_count].first_ch +
                        strlen("JMP"), " %c%d", &c, &val) == 2
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
                                if ((cmd_array[ip++] =
                                find_func(funcs, func_count, str_val)) != -1) {
                                        stack_push(&ret_ip, ip);
                                }
                        }
                } else if(strcasecmp(cmd_name, "RET") == 0) {
                        cmd_array[ip++] = CMD_RET;
                        stack_pop(&ret_ip, &cmd_array[ip++]);
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
                        if (find_label(labels, label_count, cmd_name) == -1 &&
                            find_func (funcs,  func_count,  cmd_name) == -1) {
                                if (cycle_count > 0) {
                                        fprintf(stderr, "Error: Couldn't find "
                                                "command '%s'\n", cmd_name);

                                        return;
                                }

                                if (!make_label(labels, ip, &label_count, cmd_name))
                                        make_func(funcs, ip,  &func_count, cmd_name);
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

