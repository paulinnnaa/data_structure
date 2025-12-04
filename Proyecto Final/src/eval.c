/* eval.c - C90 compatible */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "eval.h"
#include "stack.h"
#include "tokenizer.h"
#include "list.h"

int eval_postfix(List *postfix, double *out, FILE *log)
{
    Stack pila;
    ListNode *n;
    double *a;
    double *b;
    double *res;
    double r;
    char op;
    Token *t;

    /* Inicializar pila */
    stack_init(&pila, free);

    n = list_head(postfix);

    fprintf(log, "--- INICIO EVALUACION POSTFIJA ---\n");
    printf("--- INICIO EVALUACION POSTFIJA ---\n");

    while (n != NULL) {
        t = (Token*) n->data;
        if (t == NULL) {
            stack_destroy(&pila);
            return 0;
        }

        if (t->type == TOKEN_NUM) {
            double *v;
            v = (double*) malloc(sizeof(double));
            if (v == NULL) {
                stack_destroy(&pila);
                return 0;
            }
            *v = atof(t->text);
            stack_push(&pila, v);

            fprintf(log, "Push numero: %s\n", t->text);
            printf("Push numero: %s\n", t->text);
        }
        else if (t->type == TOKEN_OP) {
           
            if (stack_pop(&pila, (void**)&b) != 0) { 
            }
        }
        n = list_next(n);
    }

   

    stack_destroy(&pila);
    stack_init(&pila, free);
    n = list_head(postfix);

    while (n != NULL) {
        t = (Token*) n->data;

        if (t->type == TOKEN_NUM) {
            double *v = (double*) malloc(sizeof(double));
            if (!v) { stack_destroy(&pila); return 0; }
            *v = atof(t->text);
            stack_push(&pila, v);
        } else if (t->type == TOKEN_OP) {
            int popres;
            /* Pop B */
            popres = stack_pop(&pila, (void**)&b);
            if (popres != 0) { stack_destroy(&pila); return 0; }
            /* Pop A */
            popres = stack_pop(&pila, (void**)&a);
            if (popres != 0) { free(b); stack_destroy(&pila); return 0; }

            op = t->text[0];
            if (op == '+') r = (*a) + (*b);
            else if (op == '-') r = (*a) - (*b);
            else if (op == '*') r = (*a) * (*b);
            else if (op == '/') {
                if (*b == 0.0) {
                    free(a); free(b); stack_destroy(&pila);
                    fprintf(log, "Error: division por cero\n");
                    printf("Error: division por cero\n");
                    return 0;
                }
                r = (*a) / (*b);
            }
            else if (op == '^') {
                /* pow from math.h */
                r = pow(*a, *b);
            } else {
                /* operador desconocido */
                free(a); free(b); stack_destroy(&pila);
                return 0;
            }

            fprintf(log, "Operacion: %.10g %c %.10g = %.10g\n", *a, op, *b, r);
            printf("Operacion: %.10g %c %.10g = %.10g\n", *a, op, *b, r);

            free(a);
            free(b);

            res = (double*) malloc(sizeof(double));
            if (res == NULL) { stack_destroy(&pila); return 0; }
            *res = r;
            stack_push(&pila, res);
        } else {
           
        }

        n = list_next(n);
    }

    if (stack_pop(&pila, (void**)&res) != 0) {
        stack_destroy(&pila);
        return 0;
    }

    *out = *res;
    free(res);

    while (stack_size(&pila) > 0) {
        void *tmp;
        stack_pop(&pila, &tmp);
        free(tmp);
    }

    stack_destroy(&pila);

    fprintf(log, "--- FIN EVALUACION POSTFIJA ---\n");
    printf("--- FIN EVALUACION POSTFIJA ---\n");

    return 1;
}

