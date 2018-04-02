#ifndef SYMTAB_H
#define SYMTAB_H

struct AST;   // forward declaration

struct symnode {
    char* name;
    struct AST* node;
    struct symnode* next;
    struct symnode* prev;
};

struct symtab {
    struct symtab* prevscope;
    struct symnode* head;
    struct symnode* tail;
};

struct symtab* enterscope(struct symtab*);
struct symtab* exitscope(struct symtab*);
struct AST* lookup(struct symtab*, char*);
struct AST* probe(struct symtab*, char*);
struct symtab* add_symbol(struct symtab*, char*, struct AST*);

#endif

