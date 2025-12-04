
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "dlist.h"

typedef enum {
    TOKEN_NUM,      /* number token */
    TOKEN_OP,       /* operator token */
    TOKEN_LPAREN,   /* '(' */
    TOKEN_RPAREN,   /* ')' */
    TOKEN_END,      /* end token */
    TOKEN_ERR
} TokenType;

typedef struct {
    TokenType type;
    char text[32];  /* textual form for operators and numbers (string) */
} Token;

/* tokenize expression into tokens (DList of Token*).
   Returns 1 on success, 0 on error. Caller must initialize the DList. */
int tokenize(const char *expr, DList *tokens);

/* helper to print a DList of Token* */
void print_tokens(DList *tokens);

#endif

