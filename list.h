#ifndef LIST_H
#define LIST_H

#include"ast.h"

struct node {
    AST* data;
    struct node* next;
};

struct list {
    struct node* head;
    struct node* tail;
};

typedef struct list list;

list* nil_list();
list* single_list(AST* ast);
list* append_list(list* l1, list* l2);

#endif

