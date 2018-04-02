#include "list.h"
#include "pretty_print.h"
#include "semant.h"
#include "cgen.h"
#include <stdlib.h>

extern int yyparse();
list* program = 0;

#if 0
#include "ast_cons.h"
#include "symtab.h"
#include "utils.h"

void test_scope()
{
    struct symtab* stab = NULL;
    
    stab = enter_scope(stab);
    stab = add_symbol(stab, variable("a"));
    stab = add_symbol(stab, variable("b"));
    
    // new scope
    stab = enter_scope(stab);
    stab = add_symbol(stab, variable("a"));
    stab = add_symbol(stab, variable("b"));
    stab = add_symbol(stab, variable("c"));
    dump_symtab(stab);
    
    stab = enter_scope(stab);
    stab = add_symbol(stab, variable("x"));
    stab = add_symbol(stab, variable("y"));
    stab = add_symbol(stab, variable("z"));
    dump_symtab(stab);
    
    stab = exit_scope(stab);
    stab = exit_scope(stab);
    stab = exit_scope(stab);
    dump_symtab(stab);
}
#endif

int main()
{
    #if 1
    yyparse();
    /*for (struct node* tmp = program->head; tmp != NULL; tmp = tmp->next)
         pretty_print(tmp->data);
      */
    analyze(program);
    codegen(program);
    #endif
    
    //test_scope();
}
