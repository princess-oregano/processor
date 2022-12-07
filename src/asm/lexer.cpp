#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lexer.h"
#include "../log.h"

static const double THRESHOLD = 1e-10;

bool are_equal(double value1, double value2)
{
        return (fabs(value1 - value2) < THRESHOLD);
}

static int
lex_alloc(tok_arr_t *tok_arr, int cap)
{
        if (cap < 0) {
                log("Capacity must be larger or equal to 0.\n");
                return LEX_BAD_CAP;
        }

        token_t *tmp = nullptr;
        tmp = (token_t *) realloc(tok_arr->ptr, (size_t) cap * sizeof(token_t));

        if (tmp == nullptr) {
                log("Couldn't allocate momry for tokens.\n");
                return LEX_ALLOC;
        }

        tok_arr->ptr = tmp;
        tok_arr->cap = cap;

        return LEX_NO_ERR;
}

static ssize_t
lex_read(char *buffer, token_t *tokens)
{
        assert(buffer);
        assert(tokens);

        int i = 0;

        char *str = nullptr;
        str = strpbrk(buffer, "()");
        ssize_t len = str - buffer;

        // Check if number.
        bool number = true;
        while(i < len) {            // Later add support of float.
                if (!isdigit(buffer[i])) {
                        number = false;
                        break;
                }
                i++;
        }

        char *dummy = nullptr;
        if (number) {
                tokens->type = TOK_IMM;
                tokens->val.imm = strtof(buffer, &dummy);
                return len;
        }

        return LEX_NO_ERR;
}

static int
lex_arg(line_t line, cmd_t cmd, tok_arr_t *tok, int *count)
{
        if (cmd == CMD_PUSH)
                line.first_ch += strlen("PUSH");
        else
                line.first_ch += strlen("POP");

        // Clean whitespace.
        while (isspace(*line.first_ch))
                line.first_ch++;

        // Decide if RAM is used.
        bool ram = false;
        if (*line.first_ch == ']') {
                log("Invalid usage: cannot start arg with a ']'\n");
                return LEX_INV_USG;
        }

        if (*line.first_ch == '[') {
                if (*line.last_ch != ']') {
                        log("Invalid usage: opening brace without closing.\n");
                        return LEX_INV_USG;
                }
                ram = true;
                line.first_ch++;
        }

        // Check if number.
        // NOTE: Even in form like 3+rax, number goes first.
        bool number = false;
        char *ch = line.first_ch;
        for ( ; ch != line.last_ch && *ch != '+'; ch++) {
                if (!isdigit(*ch) && *ch != '.')
                        number = false;
        }

        double num = 0;
        if (number) {
                num = strtof(line.first_ch, &line.first_ch);
        }

        // Check if number AND register.
        // Basically checks, if '+' links number and register.
        bool num_reg = false;
        if (*line.first_ch == '+') {
                if (!are_equal(num, (int) num)) {
                        log("Invalid usage: cannot add floating-point value "
                            "to a register");
                        return LEX_INV_USG;
                }
                num_reg = true;
                num = (int) num;
                line.first_ch++;
        }

        // Check if register.
        bool reg = false;
        int reg_val = 0;
        if (line.first_ch != line.last_ch) {
                int len = line.last_ch - line.first_ch;
                if (strncasecmp(line.first_ch, "RAX", len) == 0)
                        reg_val = REG_RAX;
                else if (strncasecmp(line.first_ch, "RBX", len) == 0)
                        reg_val = REG_RBX;
                else if (strncasecmp(line.first_ch, "RCX", len) == 0)
                        reg_val = REG_RCX;
                else if (strncasecmp(line.first_ch, "RDX", len) == 0)
                        reg_val = REG_RDX;
                else {
                        log("Invalid usage: unknown sequence. "
                            "Register expected.\n");
                        return LEX_INV_USG;
                }
                reg = true;
        }

        // Form a value to push to tok_arr.
        int cmd_val = cmd;
        if (ram) {
                cmd_val |= RAM_MASK;
        }
        if (reg) {
                cmd_val |= REG_MASK;
        }
        if (number) {
                cmd_val |= IMMED_MASK;
        }

        tok[*count].ptr->val.cmd = (double) cmd_val;

        return LEX_NO_ERR;
}

int
lexer(text_t *text, tok_arr_t *tok_arr)
{
        assert(text);
        assert(tok_arr);

        lex_alloc(tok_arr, 100);

        char cmd_name[MAX_CMD_SIZE] = "";
        int i = 0;
        ssize_t lex_ret = 0;
        int tok_count = 0;
        int line_count = 0;
        while (line_count < text->num_of_lines) {
                sscanf(text->lines[line_count].first_ch, "%s", cmd_name);

                if (strcasecmp(cmd_name, "PUSH") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_PUSH;
                        lex_arg(text->lines[line_count], CMD_PUSH);
                } else if (strcasecmp(cmd_name, "POP") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_POP;
                        lex_arg();
                } else if (strcasecmp(cmd_name, "JMP") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_JMP;
                        lex_lbl();
                } else if(strcasecmp(cmd_name, "CALL") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_CALL;
                        lex_lbl();
                } else if(strcasecmp(cmd_name, "RET") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_RET;
                } else if (strcasecmp(cmd_name, "SQRT") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_SQRT;
                } else if (strcasecmp(cmd_name, "ADD") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_ADD;
                } else if (strcasecmp(cmd_name, "SUB") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_SUB;
                } else if (strcasecmp(cmd_name, "MUL") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_MUL;
                } else if (strcasecmp(cmd_name, "DIV") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_DIV;
                } else if (strcasecmp(cmd_name, "OUT") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_OUT;
                } else if (strcasecmp(cmd_name, "DMP") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_DMP;
                } else if (strcasecmp(cmd_name, "DUP") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_DUP;
                } else if (strcasecmp(cmd_name, "IN") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_IN;
                } else if (strcasecmp(cmd_name, "HLT") == 0) {
                        tok_arr[i].ptr->type = TOK_CMD;
                        tok_arr[i].ptr->val.cmd = CMD_HLT;
                } else {

                }
                tok_count++;
                i++;

                if (tok_arr->cap < tok_count + 5) {
                        lex_alloc(tok_arr, tok_arr->cap * 2);
                }

        }

        return LEX_NO_ERR;
}

