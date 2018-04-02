#ifndef UTILS_H
#define UTILS_H

#include "symtab.h"

void dump_symtab(struct symtab* stab);
void indentation(int indent);
void emit1(char*);
void emit2(char*, char*);
void emit3(char*, char*, char*);
void emit_label(char*);
char* new_label();

#endif

