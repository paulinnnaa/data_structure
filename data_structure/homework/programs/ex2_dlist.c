// P3E - Lista de ligadura doble
#include <stdio.h>
#include <stdlib.h>

#ifndef DLIST_H
#define DLIST_H

// Macros
#define dlist_size(list) ((list)->size)
#define dlist_head(list) ((list)->head)
#define dlist_tail(list) ((list)->tail)

#define dlist_is_head(node) ((node)->prev == NULL ? 1 : 0)
#define dlist_is_tail(node) ((node)->next == NULL ? 1 : 0)

#define dlist_data(node) ((node)->data)
#define dlist_next(node) ((node)->next)
#define dlist_prev(node) ((node)->prev)

typedef struct DListNode_ {
    void *data;
    struct DListNode_ *next;
    struct DListNode_ *prev;
} DListNode;

typedef struct DList_ {
    int size;
    
    void (*destroy) (void *data);
    
    DListNode *head;
    DListNode *tail;
} DList;

// Prototipos
void dlist_init (DList *list, void (*destroy)(void *data));
void dlist_destroy (DList *list);
int dlist_ins_next (DList *list, DListNode *node, const void *data);
int dlist_ins_prev (DList *list, DListNode *node, const void *data);
int dlist_remove (DList *list, DListNode *node, void **data);

#endif

static void print_list (const DList *list) {
    DListNode *node;
    int *data, i;

    fprintf(stdout, "DList size is %d\n", dlist_size(list));

    i = 0;
    node = dlist_head(list);

    while (1) {
        data = dlist_data(node);
        fprintf(stdout, "dlist.node[%03d]=%03d, %14p <- %p -> %p \n", i, *data, node->prev, node, node->next);

        i++;

        if (dlist_is_tail(node))
            break;
        else
            node = dlist_next(node);
    }

   return;
}

int main (int argc, char **argv) {
    DList list;
    DListNode *node;

    int *data, i;

    // Initialize the linked list
    dlist_init(&list, free);

    // Fill the linked list (cmd)
	for (i = 1; i < argc; i++){
	    if ((data = (int *)malloc(sizeof(int))) == NULL)
	        return 1;
	
	    *data = atoi(argv[i]);
	
	    // Insert at the end of th list
	    if (dlist_ins_next(&list, dlist_tail(&list), data) != 0)
	        return 1;
	}

    print_list(&list);

    node = dlist_head(&list);

    for (i = 0; i < 7; ++i)
        node = dlist_next(node);

    data = dlist_data(node);
    fprintf(stdout, "\nRemoving a node after the one containing %03d\n", *data);

    if (dlist_remove(&list, node, (void**)&data) != 0)
        return 1;

    print_list(&list);

    fprintf(stdout, "\nInserting 187 at the tail of the list\n");
    *data = 187;
    if (dlist_ins_next(&list, dlist_tail(&list), data) != 0)
        return 1;
    print_list(&list);

    fprintf(stdout, "\nRemoving a node at the tail of the list\n");
    if (dlist_remove(&list, dlist_tail(&list), (void**)&data) != 0)
        return 1;
    print_list(&list);

    fprintf(stdout, "\nInsert 975 before the tail of the list\n");
    *data = 975;
    if (dlist_ins_prev(&list, dlist_tail(&list), data) != 0)
        return 1;
    print_list(&list);    

    fprintf(stdout, "\nIterating and removing the fifth node\n");

    node = dlist_head(&list);
    node = dlist_next(node);
    node = dlist_prev(node);
    node = dlist_next(node);
    node = dlist_next(node);
    node = dlist_next(node);
    node = dlist_next(node);
    node = dlist_prev(node);

    if (dlist_remove(&list, node, (void **)&data) != 0)
        return 1;

    print_list(&list);

    fprintf(stdout, "\nInserting 607 before the head node\n");
    *data = 607;
    if (dlist_ins_prev(&list, dlist_head(&list), data) != 0)
        return 1;
    print_list(&list);
    
    fprintf(stdout, "\nRemoving a node at the head of the list\n");
    if (dlist_remove(&list, dlist_head(&list), (void**)&data) != 0)
        return 1;
    print_list(&list);    

    fprintf(stdout, "\nInserting 900 after the head node\n");
    *data = 900;
    if (dlist_ins_next(&list, dlist_head(&list), data) != 0)
        return 1;
    print_list(&list);

    fprintf(stdout, "\nInserting 080 two nodes after the head of the list\n");
    if ((data = (int *)malloc(sizeof(int))) == NULL)
        return 1;
    *data = 80;
    node = dlist_head(&list);
    node = dlist_next(node);
    if (dlist_ins_next(&list, node, data) != 0)
        return 1;

    print_list(&list);


    i = dlist_is_head(dlist_head(&list));
    fprintf(stdout, "\nTesting list_is_head... value=%d (1=OK)\n", i);
    i = dlist_is_head(dlist_tail(&list));
    fprintf(stdout, "Testing list_is_head... value=%d (1=OK)\n", i);
    i = dlist_is_tail(dlist_tail(&list));
    fprintf(stdout, "Testing list_is_tail... value=%d (1=OK)\n", i);
    i = dlist_is_tail(dlist_head(&list));
    fprintf(stdout, "Testing list_is_tail... value=%d (1=OK)\n", i);
    

    // Destroying the list
    fprintf(stdout, "\nDestroying the list\n");
    dlist_destroy(&list);

    return 0;
}

// Initialize the dlist
void dlist_init (DList *list, void (*destroy)(void *data)) {
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;

    return;    
}

// Destroying the dlist
void dlist_destroy (DList *list) {
    void *data;

    while(dlist_size(list) > 0) {
        if (dlist_remove(list, dlist_tail(list), (void **)&data) == 0 && list->destroy != NULL) {
            list->destroy(data);
        }
    }    
    memset(list, 0, sizeof(DList));
    return;
}

// Insert next node at the dlist
int dlist_ins_next (DList *list, DListNode *node, const void *data ) {
    DListNode    *new_node;

    // Do not allow a NULL node unless the list is empty
    if (node == NULL && dlist_size(list) != 0)
        return -1;

    if ((new_node = (DListNode *)malloc(sizeof(DListNode))) == NULL)
        return -1;

    new_node->data = (void *)data;

    // The list is empty, insert at the head
    if (dlist_size(list) == 0){
        list->head = new_node;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = new_node;
        
    } else {
        new_node->next = node->next;
        new_node->prev = node;

        if (node->next == NULL)
            list->tail = new_node;
        else
            node->next->prev = new_node;

        node->next = new_node;
    }

    list->size++;

    return 0;
}

// Insert next node at the List
int dlist_ins_prev (DList *list, DListNode *node, const void *data ) {
    DListNode    *new_node;

    // Do not allow a NULL node unless the list is empty
    if (node == NULL && dlist_size(list) != 0)
        return -1;

    if ((new_node = (DListNode *)malloc(sizeof(DListNode))) == NULL)
        return -1;

    new_node->data = (void *)data;

    // The list is empty, insert at the head
    if (dlist_size(list) == 0){
        list->head = new_node;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = new_node;
        
    } else {
        new_node->next = node;
        new_node->prev = node->prev;

        if (node->prev == NULL)
            list->head = new_node;
        else
            node->prev->next = new_node;

        node->prev = new_node;
    }

    list->size++;

    return 0;
}

// Remove node at the List
int dlist_remove (DList *list, DListNode *node, void **data) {

    // Check if is not empty the list or head-tail node
    if (node == NULL && dlist_size(list) == 0)
        return -1;
    
    *data = node->data;

    if (node == list->head) {
        list->head = node->next;

        if (list->head == NULL)
            list->tail = NULL;
        else 
            node->next->prev = NULL;

    } else {
        node->prev->next = node->next;

        if (node->next == NULL)
            list->tail = node->prev;
        else
            node->next->prev = node->prev;
    }

    free(node);
    list->size--;

    return 0;
}
