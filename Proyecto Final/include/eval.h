#ifndef EVAL_H
#define EVAL_H

#include "list.h"
#include <stdio.h>

/* evaluate postfix list (List of Token*). Writes steps to log.
   Returns 1 on success, 0 on error. Result stored in *out */
int eval_postfix(List *postfix, double *out, FILE *log);

#endif

