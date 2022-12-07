#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "../text.h"
#include "../cmd.h"

enum lex_err {
        LEX_NO_ERR  = 0,  // No error.
        LEX_ALLOC   = 1,  // Allocation error.
        LEX_BAD_CAP = 2,  // Invalid capacity for tok_arr.
        LEX_INV_USG = 3,  // Invalid usage.
};

const size_t MAX_CMD_SIZE = 40;
const int LABELS_NUM = 50;
const int LABEL_SIZE = 40;

enum token_type_t {
        TOK_CMD = 0, // Command.
        TOK_IMM = 1, // Number.
        TOK_LBL = 2, // Label.
        TOK_BRC = 3, // Brace.
        TOK_OP  = 4, // Operetion.
};

union token_val_u {
        double cmd;
        double imm;
        char lbl[LABEL_SIZE];
        bool closed;
        int sym;
};

struct token_t {
        token_type_t type = TOK_CMD;
        token_val_u val {};
};

struct tok_arr_t {
        token_t *ptr = nullptr;
        int cap = 0;
};

// Builds an array of tokens.
int
lexer(text_t *text, tok_arr_t *tok_arr);

#endif // LEXER_H

