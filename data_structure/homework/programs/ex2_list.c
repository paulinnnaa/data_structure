// P3E - Lista de ligadura sencilla (caracteres)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LIST_H
#define LIST_H

// Macros
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)

#define list_is_head(list, node) ((node) == (list)->head ? 1 : 0)
#define list_is_tail(node) ((node)->next == NULL ? 1 : 0)

#define list_data(node) ((node)->data)
#define list_next(node) ((node)->next)

typedef struct ListNode_ {
    void *data;
    struct ListNode_ *next;
} ListNode;

typedef struct List_ {
    int size;
    
    void (*destroy) (void *data);
    
    ListNode *head;
    ListNode *tail;
} List;

// Prototipos
void list_init (List *list, void (*destroy)(void *data));
void list_destroy (List *list);
int list_ins_next (List *list, ListNode *node, const void *data);
int list_rem_next (List *list, ListNode *node, void **data);

#endif

// Imprime
static void print_list (const List *list) {
    ListNode *node;
    char *data;
    int i;

    fprintf(stdout, "List size is %d\n", list_size(list));

    i = 0;
    node = list_head(list);

    while (1) {
        data = (char *)list_data(node);
        fprintf(stdout, "list.node[%03d] = '%c', %p -> %p \n", i, *data, node, node->next);

        i++;

        if (list_is_tail(node))
            break;
        else
            node = list_next(node);
    }

   return;
}


int main (int argc, char **argv) {
    List list;
    ListNode *node;
	char *data;
    int i;

    // Initialize the linked list
    list_init(&list, free);

    // List of 27 letters
    for (i = 0; i < 26; i++) {
        if ((data = (char *)malloc(sizeof(char))) == NULL)
            return 1;
        *data = 'A' + i;
        if (list_ins_next(&list, NULL, data) != 0)
            return 1;
    }

    // Last character (space)
    if ((data = (char *)malloc(sizeof(char))) == NULL)
        return 1;
    *data = ' ';
    if (list_ins_next(&list, NULL, data) != 0)
        return 1;

    print_list(&list);

    node = list_head(&list);

    for (i = 0; i < 7; ++i)
        node = list_next(node);

    data = (char *)list_data(node);
    fprintf(stdout, "\nRemoving a node after the one containing '%c'\n", *data);

    if (list_rem_next(&list, node, (void**)&data) != 0)
        return 1;

    print_list(&list);

    fprintf(stdout, "\nInserting 'W' at the tail of the list\n");
    data = (char *)malloc(sizeof(char));
    *data = 'W';
    if (list_ins_next(&list, list_tail(&list), data) != 0)
        return 1;
        
    print_list(&list);

    fprintf(stdout, "\nRemoving a node after the first node\n");
    if (list_rem_next(&list, list_head(&list), (void**)&data) != 0)
        return 1;
    print_list(&list);

    fprintf(stdout, "\nRemoving a node at the head of the list\n");
    if (list_rem_next(&list, NULL, (void**)&data) != 0)
        return 1;
    print_list(&list);
    
    fprintf(stdout, "\nInsert 'P' at the head of the list\n");
    data = (char *)malloc(sizeof(char));
    *data = 'P';
    if (list_ins_next(&list, NULL, data) != 0)
        return 1;
    print_list(&list);    

    fprintf(stdout, "\nIterating and removing the fourth node\n");

    node = list_head(&list);
    node = list_next(node);
    node = list_next(node);

    if (list_rem_next(&list, node, (void **)&data) != 0)
        return 1;

    print_list(&list);

    fprintf(stdout, "\nInserting 'Z' after the first node\n");
    data = (char *)malloc(sizeof(char));
    *data = 'Z';
    if (list_ins_next(&list, list_head(&list), data) != 0)
        return 1;
    print_list(&list);

    i = list_is_head(&list, list_head(&list));
    fprintf(stdout, "\nTesting list_is_head... value=%d (1=OK)\n", i);
    i = list_is_head(&list, list_tail(&list));
    fprintf(stdout, "Testing list_is_head... value=%d (1=OK)\n", i);
    i = list_is_tail(list_tail(&list));
    fprintf(stdout, "Testing list_is_tail... value=%d (1=OK)\n", i);
    i = list_is_tail(list_head(&list));
    fprintf(stdout, "Testing list_is_tail... value=%d (1=OK)\n", i);
    

    // Destroying the list
    fprintf(stdout, "\nDestroying the list\n");
    list_destroy(&list);

    return 0;
}

// Inicializa
void list_init (List *list, void (*destroy)(void *data)) {
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;

    return;    
}

// Destruye
void list_destroy (List *list) {
    void *data;

    while(list_size(list) > 0) {
        if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
            list->destroy(data);
        }
    }    
    memset(list, 0, sizeof(List));
    return;
}

// Inserta 
int list_ins_next (List *list, ListNode *node, const void *data ) {
    ListNode *new_node;

    if ((new_node = (ListNode *)malloc(sizeof(ListNode))) == NULL)
        return -1;

    new_node->data = (void *)data;

    // Handle insertion from head at the list
    if (node == NULL) {
        
        // The list is empty
        if (list_size(list) == 0)
            list->tail = new_node;

        // There are other nodes in the list
        // do an adjust of other nodes
        new_node->next = list->head;
        list->head = new_node;
        
    } else {
        // Handle insertion from somewhere other than at the head

        // If the node is insert in the 
        if (node->next == NULL)
            list->tail = new_node;

        // Between two nodes at the list
        // do an adjust of the other nodes
        new_node->next = node->next;
        node->next = new_node;
    }

    list->size++;

    return 0;
}

// Borra
int list_rem_next (List *list, ListNode *node, void **data) {
    ListNode *old_node;

    // Check if is not empty the list
    if (list_size(list) == 0)
        return -1;
    
    // Handle removal from head of the list 
    if (node == NULL) {
    
        *data = list->head->data;
        old_node = list->head;
        list->head = list->head->next;

        // Is the last node at the list?
        if (list_size(list) == 1)
            list->tail = NULL;

    } else {
        // Handle removal from somewhere other than at the 

        // Can not remove at the end of the list
        if (node->next == NULL)
            return -1;

        *data = node->next->data;
        old_node = node->next;
        node->next = node->next->next;
    }

    free(old_node);
    list->size--;

    return 0;
}
