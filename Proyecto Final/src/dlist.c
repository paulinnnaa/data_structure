
#include <stdlib.h>
#include "dlist.h"

void dlist_init(DList *list, void (*destroy)(void *data)) {
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

void dlist_destroy(DList *list) {
    void *data;
    while (list->size > 0) {
        dlist_remove(list, list->tail, &data);
        if (list->destroy != NULL) list->destroy(data);
    }
    list->head = NULL;
    list->tail = NULL;
}

int dlist_ins_next(DList *list, DListNode *node, const void *data) {
    DListNode *new_node;
    if (node == NULL && list->size != 0) return -1;
    new_node = (DListNode*) malloc(sizeof(DListNode));
    if (new_node == NULL) return -1;
    new_node->data = (void*) data;
    if (list->size == 0) {
        list->head = new_node;
        list->tail = new_node;
        new_node->prev = NULL;
        new_node->next = NULL;
    } else {
        new_node->next = node->next;
        new_node->prev = node;
        if (node->next == NULL) list->tail = new_node;
        else node->next->prev = new_node;
        node->next = new_node;
    }
    list->size++;
    return 0;
}

int dlist_ins_prev(DList *list, DListNode *node, const void *data) {
    DListNode *new_node;
    if (node == NULL && list->size != 0) return -1;
    new_node = (DListNode*) malloc(sizeof(DListNode));
    if (new_node == NULL) return -1;
    new_node->data = (void*) data;
    if (list->size == 0) {
        list->head = new_node;
        list->tail = new_node;
        new_node->prev = NULL;
        new_node->next = NULL;
    } else {
        new_node->next = node;
        new_node->prev = node->prev;
        if (node->prev == NULL) list->head = new_node;
        else node->prev->next = new_node;
        node->prev = new_node;
    }
    list->size++;
    return 0;
}

int dlist_remove(DList *list, DListNode *node, void **data) {
    if (node == NULL || list->size == 0) return -1;
    *data = node->data;
    if (node == list->head) {
        list->head = node->next;
        if (list->head == NULL) list->tail = NULL;
        else node->next->prev = NULL;
    } else {
        node->prev->next = node->next;
        if (node->next == NULL) list->tail = node->prev;
        else node->next->prev = node->prev;
    }
    free(node);
    list->size--;
    return 0;
}

