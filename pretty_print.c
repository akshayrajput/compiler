#include "pretty_print.h"
#include "list.h"
#include "type.h"
#include "utils.h"

#include <stdio.h>

static void pprint_(AST* ast, int indent);
static void pprint(AST* ast, int indent);

void pretty_print(AST* ast)
{
    pprint(ast, 0);
}

static void pprint(AST* ast, int indent)
{
    if (ast != NULL)
        pprint_(ast, indent);
}

static void print_binexpr(int indent, char* rep, AST* ast)
{
    indentation(indent);
    printf("%s\n",rep);
    pprint(ast->d.be.lhs_, indent+2);
    pprint(ast->d.be.rhs_, indent+2);
}

static void pprint_(AST* ast, int indent)
{
    switch(ast->kind_)
    {
        case TOKEN_K_NUM: indentation(indent);
                    printf("%d\n",ast->d.val_);
                    break;
                    
        case TOKEN_BE_LT: print_binexpr(indent, "<", ast);
                    break;
                    
        case TOKEN_BE_LTE: print_binexpr(indent, "<=", ast);
                    break;
                    
        case TOKEN_BE_GT: print_binexpr(indent, ">", ast);
                    break;

        case TOKEN_BE_GTE: print_binexpr(indent, ">=", ast);
                    break;

        case TOKEN_BE_EQ: print_binexpr(indent, "==", ast);
                    break;

        case TOKEN_BE_NEQ: print_binexpr(indent, "!=", ast);
                    break;

        case TOKEN_BE_ADD: print_binexpr(indent, "+", ast);
                    break;

        case TOKEN_BE_SUB: print_binexpr(indent, "-", ast);
                    break;

        case TOKEN_BE_MUL: print_binexpr(indent, "*", ast);
                    break;

        case TOKEN_BE_DIV: print_binexpr(indent, "/", ast);
                    break;
        
        case TOKEN_K_CALL: indentation(indent);
                     printf("callee: %s\n", ast->d.call.name_);
                     for (struct node* tmp = ast->d.call.args_->head; tmp != NULL; tmp = tmp->next)
                        pprint(tmp->data, indent+2);
                    break;
                    
        case TOKEN_E_ASSIGN: indentation(indent);
                       printf("=\n");
                       pprint(ast->d.assign.to_, indent+2);
                       pprint(ast->d.assign.expr_, indent+2);
                    break;
        
        case TOKEN_ARRAY_ACCESS: indentation(indent);
                           printf("Array name: %s\n",ast->d.access.name_);
                           pprint(ast->d.access.expr_, indent+2);
                    break;
                    
        case TOKEN_VAR_NAME: indentation(indent);
                       printf("variable name: %s\n",ast->d.variable.name_);
                    break;
       
        case TOKEN_RETURN:  indentation(indent);
                      printf("Return statement\n");
                      pprint(ast->d.ret.expr_, indent+2);
                    break;
                    
        case TOKEN_WHILE:   indentation(indent);
                      printf("While statement\n");
                      pprint(ast->d.while_loop.expr_, indent+2);
                      pprint(ast->d.while_loop.stmt_, indent+2);
                    break;
                    
        case TOKEN_IF_THEN:  indentation(indent);
                       printf("If then statement\n");
                       pprint(ast->d.if_then.expr_, indent+2);
                       pprint(ast->d.if_then.if_stmt_, indent+2);
                       if(ast->d.if_then.else_stmt_ != NULL)
                       {
                            indentation(indent);
                            printf("else\n");
                            pprint(ast->d.if_then.else_stmt_, indent+2);
                       }
                    break;
    
        case TOKEN_COMPOUND: indentation(indent);
                       printf("Declarations\n");
                       for (struct node* tmp = ast->d.compound.decls_->head; tmp != NULL; tmp = tmp->next)
                          pprint(tmp->data, indent+2);
                       
                       indentation(indent);
                       printf("Statements\n");
                       for (struct node* tmp = ast->d.compound.stmts_->head; tmp != NULL; tmp = tmp->next)
                          pprint(tmp->data, indent+2);
                    break;
                    
        case TOKEN_PARAM:  indentation(indent);
                     printf("%s %s%s\n",
                        ast->d.param.type_spec_ == TYPE_INT ? "int" : "void",
                        ast->d.param.name_,
                        ast->d.param.ref_ == 0 ? "" : " []");
                    break;
                  
        case TOKEN_FUNCTION: indentation(indent);
                       printf("%s %s\n",
                            ast->d.function.ret_type_ ==  TYPE_INT ? "int" : "void", ast->d.function.name_);
                       indentation(indent+2);
                       
                       printf("Parameters\n");
                       for (struct node* tmp = ast->d.function.params_->head; tmp != NULL; tmp = tmp->next)
                          pprint(tmp->data, indent+2);
                       
                       pprint(ast->d.function.body_,indent+2);
                    break;
              
        case TOKEN_VAR_DECL: indentation(indent);
                       printf("%s %s %d %s\n",
                        ast->d.var_decl.type_spec_ == TYPE_INT ? "int" : "void",
                        ast->d.var_decl.name_,
                        ast->d.var_decl.ref_ == 0 ? 1 : ast->d.var_decl.size_,
                        ast->d.var_decl.ref_ == 0 ? "" : " []");
                    break;
    }
}
