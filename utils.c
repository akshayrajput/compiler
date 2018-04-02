#include "utils.h"
#include "list.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

static void dump_cur_(struct symtab* cur, int indent)
{
    for (struct symnode* tmp = cur->head; tmp != NULL; tmp = tmp->next)
    {
        indentation(indent);
        printf("%s\n", tmp->name);
    }
}

void dump_symtab(struct symtab* stab)
{
    if (stab == NULL) return;
    
    int indent = 0;
    for (struct symtab* tmp = stab; tmp != NULL; tmp = tmp->prevscope)
    {
        indentation(indent);
        printf("scope begins:\n");
        dump_cur_(tmp, indent+2);
        indent += 4;
    }
}

void indentation(int indent)
{
    for (; indent > 0; --indent)
        putchar(' ');
}

void emit1(char* s1)
{
    printf("\t%s\n",s1);
}

void emit2(char* s1, char* s2)
{
    printf("\t%s %s\n",s1, s2);
}

void emit3(char* s1, char* s2, char* s3)
{
    printf("\t%s %s, %s\n",s1, s2, s3);
}

void emit_label(char* name)
{
    printf("%s:\n",name);
}

char* new_label()
{
    static int i = 0;
    char* buf = (char*) malloc(sizeof(char)*5);
    sprintf(buf,".L%d",i++);
    return buf;
}
