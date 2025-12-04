/* parser.c - corregido: infix_to_prefix ignora TOKEN_END al invertir */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "stack.h"
#include "tokenizer.h"
#include "list.h"
#include "dlist.h"


/* bandera global: indica si estamos generando PREFIJA */
int modo_prefija = 0;

/* precedence */
static int precedence(char op) {
    switch (op) {
        case '^': return 4;
        case '*': case '/': return 3;
        case '+': case '-': return 2;
    }
    return 0;
}

/* right associative '^' */
static int is_right_assoc(char op) {
    return (op == '^');
}

/* print current output list */
static void print_output(List *out, FILE *log) {
    ListNode *ln;

    fprintf(log, "Salida parcial: ");
    printf("Salida parcial: ");

    ln = list_head(out);
    while (ln != NULL) {
        Token *t = (Token*) ln->data;
        if (t) {
            fprintf(log, "%s ", t->text);
            printf("%s ", t->text);
        }
        ln = list_next(ln);
    }
    fprintf(log, "\n");
    printf("\n");
}

/* print operator stack */
static void print_stack(Stack *s, FILE *log) {
    ListNode *ln;

    fprintf(log, "Stack op: ");
    printf(" op: ");

    ln = list_head((List*)s);
    while (ln != NULL) {
        Token *t = (Token*) ln->data;
        if (t) {
            fprintf(log, "%s ", t->text);
            printf("%s ", t->text);
        }
        ln = list_next(ln);
    }
    fprintf(log, "\n");
    printf("\n");
}

/* copy token */
static Token *copy_token(Token *src) {
    Token *dst = (Token*) malloc(sizeof(Token));
    if (!dst) return NULL;

    memcpy(dst, src, sizeof(Token));
    return dst;
}

/* ========================================================= */
/*                INFIX ? POSTFIX                            */
/* ========================================================= */

int infix_to_postfix(DList *tokens, List *out_postfix, FILE *log) {
    Stack opstack;
    DListNode *dn;
    Token *tok;
    Token *ct;
    void *pdata;
    Token *top;
    char op;
    char topch;

    stack_init(&opstack, free);
    dn = dlist_head(tokens);

    if (!modo_prefija) {
        fprintf(log, "--- INICIO conversion a POSTFIJA ---\n");
        printf("--- INICIO conversion a POSTFIJA ---\n");
    }

    while (dn != NULL) {
        tok = (Token*) dn->data;

        if (!tok) { stack_destroy(&opstack); return 0; }

        fprintf(log, "Leyendo token: %s (tipo=%d)\n", tok->text, tok->type);
        printf("Leyendo token: %s (tipo=%d)\n", tok->text, tok->type);

        /* números */
        if (tok->type == TOKEN_NUM) {
            ct = copy_token(tok);
            if (!ct) { stack_destroy(&opstack); return 0; }

            list_ins_next(out_postfix, list_tail(out_postfix), ct);
            fprintf(log, " -> numero enviado a salida: %s\n", ct->text);
            printf(" -> numero enviado a salida: %s\n", ct->text);

            print_output(out_postfix, log);
        }

        /* '(' */
        else if (tok->type == TOKEN_LPAREN) {
            ct = copy_token(tok);
            if (!ct) { stack_destroy(&opstack); return 0; }
            stack_push(&opstack, ct);

            fprintf(log, " -> push '('\n");
            printf(" -> push '('\n");

            print_stack(&opstack, log);
        }

        /* ')' */
        else if (tok->type == TOKEN_RPAREN) {
            while (stack_size(&opstack) > 0) {
                top = (Token*) stack_peek(&opstack);
                if (top != NULL && top->type == TOKEN_LPAREN) break;

                if (stack_pop(&opstack, &pdata) != 0) { stack_destroy(&opstack); return 0; }
                list_ins_next(out_postfix, list_tail(out_postfix), pdata);

                fprintf(log, " -> pop oper %s a salida\n", ((Token*)pdata)->text);
                printf(" -> pop oper %s a salida\n", ((Token*)pdata)->text);

                print_output(out_postfix, log);
            }

            if (stack_size(&opstack) == 0) {
                fprintf(log, "Error: parentesis sin abrir\n");
                printf("Error: parentesis sin abrir\n");
                stack_destroy(&opstack);
                return 0;
            }

            if (stack_pop(&opstack, &pdata) != 0) { stack_destroy(&opstack); return 0; }
            free(pdata);

            fprintf(log, " -> descartado '('\n");
            printf(" -> descartado '('\n");

            print_stack(&opstack, log);
        }

        /* operadores */
        else if (tok->type == TOKEN_OP) {
            op = tok->text[0];

            while (stack_size(&opstack) > 0) {
                top = (Token*) stack_peek(&opstack);
                if (top != NULL && top->type == TOKEN_LPAREN) break;

                topch = top->text[0];
                if ((!is_right_assoc(op) && precedence(topch) >= precedence(op)) ||
                    (is_right_assoc(op) && precedence(topch) > precedence(op))) {

                    if (stack_pop(&opstack, &pdata) != 0) { stack_destroy(&opstack); return 0; }
                    list_ins_next(out_postfix, list_tail(out_postfix), pdata);

                    fprintf(log, " -> pop por precedencia %s a salida\n", ((Token*)pdata)->text);
                    printf(" -> pop por precedencia %s a salida\n", ((Token*)pdata)->text);

                    print_output(out_postfix, log);
                    continue;
                }
                break;
            }

            ct = copy_token(tok);
            if (!ct) { stack_destroy(&opstack); return 0; }

            stack_push(&opstack, ct);

            fprintf(log, " -> push oper %s\n", ct->text);
            printf(" -> push oper %s\n", ct->text);

            print_stack(&opstack, log);
        }

        /* END */
        else if (tok->type == TOKEN_END) {
            fprintf(log, "Token END, deteniendo\n");
            printf("Token END, deteniendo\n");
            break;
        }

        dn = dlist_next(dn);
    }

    /* vaciar pila */
    while (stack_size(&opstack) > 0) {
        top = (Token*) stack_peek(&opstack);

        if (top != NULL && top->type == TOKEN_LPAREN) {
            fprintf(log, "Error: parentesis sin cerrar\n");
            printf("Error: parentesis sin cerrar\n");
            stack_destroy(&opstack);
            return 0;
        }

        stack_pop(&opstack, &pdata);
        list_ins_next(out_postfix, list_tail(out_postfix), pdata);

        fprintf(log, " -> vaciado pila: %s\n", ((Token*)pdata)->text);
        printf(" -> vaciado pila: %s\n", ((Token*)pdata)->text);

        print_output(out_postfix, log);
    }

    stack_destroy(&opstack);

    if (!modo_prefija) {
        fprintf(log, "--- FIN conversion POSTFIJA ---\n");
        printf("--- FIN conversion POSTFIJA ---\n");
    }

    return 1;
}

/* ========================================================= */
/*                   INFIX ? PREFIX                          */
/* ========================================================= */

int infix_to_prefix(DList *tokens, List *out_prefix, FILE *log) {
    DList rev;
    DListNode *dn;
    Token *orig;
    Token *cp;
    List tmp;
    ListNode *ln;
    int count;
    int idx;
    void **arr;
    int i;

    /* encabezado especial */
    fprintf(log, "--- INICIO conversion PREFIJA ---\n");
    printf("--- INICIO conversion PREFIJA ---\n");

    dlist_init(&rev, free);

    /* 1. invertir tokens sin TOKEN_END */
    dn = dlist_tail(tokens);
    while (dn != NULL) {
        orig = (Token*) dn->data;

        if (orig != NULL && orig->type == TOKEN_END) {
            dn = dlist_prev(dn);
            continue;
        }

        cp = copy_token(orig);
        if (!cp) { dlist_destroy(&rev); return 0; }

        if (cp->type == TOKEN_LPAREN) {
            cp->type = TOKEN_RPAREN;
            strcpy(cp->text, ")");
        }
        else if (cp->type == TOKEN_RPAREN) {
            cp->type = TOKEN_LPAREN;
            strcpy(cp->text, "(");
        }

        dlist_ins_next(&rev, dlist_tail(&rev), cp);
        dn = dlist_prev(dn);
    }

    /* 2. postfix del invertido */
    list_init(&tmp, free);

    
    modo_prefija = 1;
    if (!infix_to_postfix(&rev, &tmp, log)) {
        modo_prefija = 0;
        dlist_destroy(&rev);
        list_destroy(&tmp);
        return 0;
    }
    modo_prefija = 0;

    /* 3. invertir postfix final */
    count = list_size(&tmp);
    if (count > 0) {
        arr = (void**) malloc(sizeof(void*) * count);
        if (!arr) { dlist_destroy(&rev); list_destroy(&tmp); return 0; }

        idx = 0;
        ln = list_head(&tmp);
        while (ln != NULL) {
            arr[idx++] = ln->data;
            ln = list_next(ln);
        }

        for (i = count - 1; i >= 0; i--) {
            orig = (Token*) arr[i];

            cp = copy_token(orig);
            list_ins_next(out_prefix, list_tail(out_prefix), cp);
        }

        free(arr);
    }

    list_destroy(&tmp);
    dlist_destroy(&rev);

    fprintf(log, "--- FIN conversion PREFIJA ---\n");
    printf("--- FIN conversion PREFIJA ---\n");

    return 1;
}

