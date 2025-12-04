
#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "list.h"
#include "dlist.h"
#include <stdio.h>

extern int modo_prefija;

/* convert tokens list (DList) to postfix list (List of Token*), write steps to log.
   Returns 1 on success, 0 on error. */
int infix_to_postfix(DList *tokens, List *out_postfix, FILE *log);

/* convert tokens list to prefix list, write steps to log.
   Returns 1 on success, 0 on error. */
int infix_to_prefix(DList *tokens, List *out_prefix, FILE *log);

#endif

