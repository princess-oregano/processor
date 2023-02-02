#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "text2code.h"
#include "../stack.h"
#include "../log.h"

const double THRESHOLD = 1e-10;

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
        if (str[len - 1] == ':') {
                len--;
        }
        else {
                log("Invalid usage: colon expected after label.\n");
                return;
        }


        len = (len < LABEL_SIZE) ? len : LABEL_SIZE;

        labels[*size].n_label = *size;
        labels[*size].ip = ip;
        memcpy(labels[*size].name, str, len);

        *size += 1;
}

// Dumps info about labels.
[[maybe_unused]]
static void
label_dump(label_t *labels, size_t size)
{
        for (size_t i = 0; i < size; i++) {
                log("Label %zu:\n"
                    "Name '%s'\n"
                    "IP '%zu'\n", labels[i].n_label,
                                  labels[i].name,
                                  labels[i].ip);

                log("\n");
        }
}

///////////////////END_LABELS///////////////////////

#define DEF_REG(arg) if(strncasecmp(reg, #arg, 3) == 0) { \
                val = REG_##arg;                      \
                }\
        else

static int
gen_reg(char *reg)
{
        int val = 777;

        #include "../regs.inc"
        {
                log("Invalid usage: unknown argument.\n");
        }

        return val;
}

#undef DEF_REG

static void
gen_push(char *buf, double *cmd_array, size_t *ip)
{

        double val = 0;
        char str_val[40] = "";
        if (sscanf(buf, "%lf", &val) == 1) {
                cmd_array[(*ip)++] = CMD_PUSH | (IMMED_MASK);
                cmd_array[(*ip)++] = val;
        } else if (sscanf(buf, " [ %lf ] ", &val) == 1) {
                cmd_array[(*ip)++] = CMD_PUSH | (IMMED_MASK | RAM_MASK);
                cmd_array[(*ip)++] = val;
        } else if (sscanf(buf, " [ %s + %lf ] ", str_val, &val) == 2) {
                cmd_array[(*ip)++] = CMD_PUSH |
                        (IMMED_MASK | REG_MASK | RAM_MASK);
                cmd_array[(*ip)++] = val;
                cmd_array[(*ip)++] = gen_reg(str_val);
        } else if (sscanf(buf, " [ %s ] ", str_val) == 1) {
                cmd_array[(*ip)++] = CMD_PUSH | (REG_MASK | RAM_MASK);
                cmd_array[(*ip)++] = gen_reg(str_val);
        } else if (sscanf(buf, "%s", str_val) == 1) {
                cmd_array[(*ip)++] = CMD_PUSH | (REG_MASK);
                cmd_array[(*ip)++] = gen_reg(str_val);
        } else {
                log("Error: invalid PUSH usage.\n");
                return;
        }
}

static void
gen_pop(char *buf, double *cmd_array, size_t *ip)
{
        double val = 0;
        char str_val[40] = "";
        if (sscanf(buf, " [ %lf ] ", &val) == 1) {
                cmd_array[(*ip)++] = CMD_POP | (IMMED_MASK | RAM_MASK);
                cmd_array[(*ip)++] = val;
        } else if (sscanf(buf, " [ %s + %lf ] ", str_val, &val) == 2) {
                cmd_array[(*ip)++] = CMD_POP |
                        (IMMED_MASK | REG_MASK | RAM_MASK);
                cmd_array[(*ip)++] = val;
                cmd_array[(*ip)++] = gen_reg(str_val);
        } else if (sscanf(buf, " [ %s ] ", str_val) == 1) {
                cmd_array[(*ip)++] = CMD_POP | (REG_MASK | RAM_MASK);
                cmd_array[(*ip)++] = gen_reg(str_val);
        } else if (sscanf(buf, " %s ", str_val) == 1) {
                cmd_array[(*ip)++] = CMD_POP | (REG_MASK);
                cmd_array[(*ip)++] = gen_reg(str_val);
        } else {
                log("Error: invalid PUSH usage.\n");
                return;
        }
}

#define DEF_CMD(name, ...) if (strcasecmp(cmd_name, #name) == 0) { \
                        cmd_array[ip++] = CMD_##name;       \
                  } else
#define DEF_JMP(cmd, ...) if (strcasecmp(cmd_name, #cmd) == 0) { \
                        cmd_array[ip++] = CMD_##cmd;               \
                        if (sscanf(text->lines[line_count].first_ch + \
                        strlen(#cmd), " %c%s", &c, str_val) == 2 \
                        && c == ':') { \
                                cmd_array[ip++] = \
                                find_label(labels, label_count, str_val); \
                        }    \
                } else

void
generate(text_t *text, cmd_arr_t *cmd_arr)
{
        double *cmd_array = (double *) calloc(text->num_of_lines * 3, sizeof(double));

        char cmd_name[MAX_CMD_SIZE] = {};
        label_t labels[LABELS_NUM] = {};
        size_t ip = 0;
        size_t  line_count = 0;
        size_t label_count = 0;
        char str_val[40] = {};
        char c = 0;

        int cycle_count = 0;
        while (cycle_count < 2 && line_count < text->num_of_lines) {
                sscanf(text->lines[line_count].first_ch, "%s", cmd_name);

                if (strcasecmp(cmd_name, "PUSH") == 0) {
                        gen_push(text->lines[line_count].first_ch +
                                        strlen("PUSH"), cmd_array, &ip);
                } else if (strcasecmp(cmd_name, "POP") == 0) {
                        gen_pop(text->lines[line_count].first_ch +
                                        strlen("POP"), cmd_array, &ip);
                } else
                #include "../cmds.inc"
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
}

#undef DEF_CMD
#undef DEF_JMP

#define DEF_CMD(cmd, ...) case CMD_##cmd:  \
                     fprintf(list, #cmd);  \
                     break;
#define DEF_JMP(cmd, ...) case CMD_##cmd:  \
                fprintf(list, #cmd);  \
                fprintf(list, " %lg", cmd_arr.cmd_array[ip++]);  \
                break;

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
                        #include "../cmds.inc"
                        default:
                                assert(0 && "Invalid usage.");
                                break;
                }
                fprintf(list, "\n");
        }

        fprintf(list, "\n");
}

#undef DEF_CMD
#undef DEF_JMP

