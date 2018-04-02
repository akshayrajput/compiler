#include "symtab.h"
#include "list.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static struct symnode* make_symnode(char* name, AST* node)
{
    struct symnode* temp = (struct symnode*)malloc(sizeof(struct symnode*));
    temp->name = name;
    temp->node = node;
    return temp;
}

static struct symtab* make_symtab()
{
    struct symtab* temp = (struct symtab*)malloc(sizeof(struct symtab*));
    temp->prevscope = NULL;
    temp->head = NULL;
    temp->tail = NULL;
    return temp;
}

struct symtab* enterscope(struct symtab* cur)
{
    struct symtab* temp = make_symtab();
    temp->prevscope = cur;
    return temp;
}

struct symtab* exitscope(struct symtab* cur)
{
    if (cur != NULL)
        cur = cur->prevscope;
    return cur;
}

struct AST* lookup(struct symtab* cur, char* name)
{
    while (cur != NULL)
    {
        struct AST* var = probe(cur, name);
        if (var != NULL)
            return var;
        cur = cur->prevscope;
    }
    
    return NULL;
}

struct AST* probe(struct symtab* cur, char* name)
{
    for (struct symnode* tmp = cur->head; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(tmp->name, name) == 0)
            return tmp->node;
    }
    
    return NULL;
}

struct symtab* add_symbol(struct symtab* cur, char* name, struct AST* ast)
{
   
   /*struct AST* var = probe(cur, name);
   
   if (var != NULL)
   {
       fprintf(stderr, "%s\n", "error: predefined variable in the same scope");
       return cur;
   }*/
   
   struct symnode* temp= make_symnode(name, ast);
   
   if(cur->tail == NULL || cur->head == NULL)
   {
        cur->head = cur->tail = temp;
   }
   else
   {
        cur->tail->next = temp;
        temp->prev = cur->tail;
        cur->tail = temp;
   }
   return cur;
}

