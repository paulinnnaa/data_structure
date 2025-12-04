
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

/* create token */
static Token *new_token(TokenType type, const char *text) {
    Token *t = (Token*) malloc(sizeof(Token));
    if (!t) return NULL;
    t->type = type;
    strncpy(t->text, text, 31);
    t->text[31] = '\0';
    return t;
}

/* tokenize with decimals and unary minus handling
   IMPORTANT: the caller must have initialized the DList (dlist_init). 
   Returns 1 on success, 0 on error. */
int tokenize(const char *expr, DList *tokens) {
    int i = 0;
    int len = strlen(expr);
    while (i < len) {
        char c = expr[i];
        if (isspace((unsigned char)c)) { i++; continue; }

        /* parentheses */
        if (c == '(') {
            Token *t = new_token(TOKEN_LPAREN, "(");
            if (!t) return 0;
            dlist_ins_next(tokens, dlist_tail(tokens), t);
            i++; continue;
        }
        if (c == ')') {
            Token *t = new_token(TOKEN_RPAREN, ")");
            if (!t) return 0;
            dlist_ins_next(tokens, dlist_tail(tokens), t);
            i++; continue;
        }

        /* operators */
        if (strchr("+-*/^", c)) {
            /* handle unary minus: - followed by digit or dot and previous is '(' or start or operator */
            if (c == '-') {
                int prev_idx = i - 1;
                while (prev_idx >= 0 && isspace((unsigned char)expr[prev_idx])) prev_idx--;
                if (prev_idx < 0 || expr[prev_idx] == '(' || strchr("+-*/^", expr[prev_idx])) {
                    /* unary minus: parse number with leading - */
                    int j = 0;
                    char buf[32];
                    buf[j++] = '-';
                    i++;
                    while (i < len && (isdigit((unsigned char)expr[i]) || expr[i] == '.')) {
                        if (j < 31) buf[j++] = expr[i];
                        i++;
                    }
                    buf[j] = '\0';
                    Token *t = new_token(TOKEN_NUM, buf);
                    if (!t) return 0;
                    dlist_ins_next(tokens, dlist_tail(tokens), t);
                    continue;
                }
            }
            {
                char op[2];
                op[0] = c; op[1] = '\0';
                Token *t = new_token(TOKEN_OP, op);
                if (!t) return 0;
                dlist_ins_next(tokens, dlist_tail(tokens), t);
                i++; continue;
            }
        }

        /* numbers (integer or decimal) */
        if (isdigit((unsigned char)c) || c == '.') {
            int j = 0;
            char buf[32];
            while (i < len && (isdigit((unsigned char)expr[i]) || expr[i] == '.')) {
                if (j < 31) buf[j++] = expr[i];
                i++;
            }
            buf[j] = '\0';
            Token *t = new_token(TOKEN_NUM, buf);
            if (!t) return 0;
            dlist_ins_next(tokens, dlist_tail(tokens), t);
            continue;
        }

        /* unknown char */
        return 0;
    }

    /* add end token */
    Token *t = new_token(TOKEN_END, "<END>");
    if (!t) return 0;
    dlist_ins_next(tokens, dlist_tail(tokens), t);
    return 1;
}

/* print tokens in a DList (for debugging / display) */
void print_tokens(DList *tokens)
{
    DListNode *dn;
    Token *t;
    int index = 1;

    printf("\n--- TOKENIZACION ---\n\n");
    printf("#   Tipo        Texto\n");
    printf("-----------------------------\n");

    dn = dlist_head(tokens);
    while (dn != NULL)
    {
        t = (Token*) dn->data;
        if (t != NULL)
        {
            const char *tipo;

            switch (t->type)
            {
                case TOKEN_NUM: tipo = "NUM"; break;
                case TOKEN_OP:  tipo = "OP";  break;
                case TOKEN_LPAREN: tipo = "LPAREN"; break;
                case TOKEN_RPAREN: tipo = "RPAREN"; break;
                case TOKEN_END: tipo = "END"; break;
                default: tipo = "???"; break;
            }

            printf("%-3d %-10s %-10s\n", index, tipo, t->text);
        }

        dn = dlist_next(dn);
        index++;
    }

    printf("\n-----------------------------\n");
    printf("Total de tokens: %d\n\n", index - 1);
}


