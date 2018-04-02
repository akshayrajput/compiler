#include"list.h"
#include<stdlib.h>

static struct node* make_node(AST* ast)
{
  struct node* tmp = malloc(sizeof(struct node));
  tmp->data = ast;
  tmp->next = NULL;
  return tmp;
}

list* nil_list()
{
    list* tmp = (list*)malloc(sizeof(list));
    tmp->head = NULL;
    tmp->tail = NULL;
    return tmp;
}

list* single_list(AST* ast)
{
    list* tmp = nil_list();
    tmp->head = tmp->tail = make_node(ast);
    return tmp;
}

list* append_list(list* l1, list* l2)
{
  if (l1 == NULL)
    return l2;
  if (l2 == NULL)
    return l1;
  
  // if l1 is nil
  if (l1->head == NULL)
  {
    l1->head = l2->head;
    l1->tail = l2->tail;
  }
  
  else
  {
    l1->tail->next = l2->head;
    l1->tail = l2->tail;
  }
  
  return l1;
}

