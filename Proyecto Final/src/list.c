
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "tokenizer.h"

/* init list */
void list_init(List *list, void (*destroy)(void *data)) {
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

/* destroy and free data */
void list_destroy(List *list) {
    void *data;
    while (list->size > 0) {
        if (list_rem_next(list, NULL, (void**)&data) == 0 && list->destroy != NULL) {
            list->destroy(data);
        }
    }
    list->head = NULL;
    list->tail = NULL;
}

/* insert after node (NULL -> head) */
int list_ins_next(List *list, ListNode *node, const void *data) {
    ListNode *new_node;
    new_node = (ListNode*) malloc(sizeof(ListNode));
    if (new_node == NULL) return -1;
    new_node->data = (void*) data;
    if (node == NULL) {
        if (list->size == 0) list->tail = new_node;
        new_node->next = list->head;
        list->head = new_node;
    } else {
        new_node->next = node->next;
        node->next = new_node;
        if (node == list->tail) list->tail = new_node;
    }
    list->size++;
    return 0;
}

/* remove after node (NULL -> head) */
int list_rem_next(List *list, ListNode *node, void **data) {
    ListNode *old_node;
    if (list->size == 0) return -1;
    if (node == NULL) {
        old_node = list->head;
        list->head = old_node->next;
        if (list->size == 1) list->tail = NULL;
    } else {
        if (node->next == NULL) return -1;
        old_node = node->next;
        node->next = old_node->next;
        if (old_node == list->tail) list->tail = node;
    }
    *data = old_node->data;
    free(old_node);
    list->size--;
    return 0;
}

/* Print a list that stores Token* (used for postfix/prefix lists) */
void print_list(List *l)
{
    ListNode *n = list_head(l);
    while (n != NULL) {
        Token *t = (Token*) n->data;
        if (t) printf("%s ", t->text);
        n = list_next(n);
    }
    printf("\n");
}

