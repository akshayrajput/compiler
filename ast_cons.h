#ifndef AST_CONS_H
#define AST_CONS_H

#include "ast.h"
#include "list.h"

AST* lt(AST* lhs, AST* rhs);
AST* lte(AST* lhs, AST* rhs);
AST* gt(AST* lhs, AST* rhs);
AST* gte(AST* lhs, AST* rhs);
AST* eq(AST* lhs, AST* rhs);
AST* neq(AST* lhs, AST* rhs);
AST* add(AST* lhs, AST* rhs);
AST* sub(AST* lhs, AST* rhs);
AST* mul(AST* lhs, AST* rhs);
AST* divide(AST* lhs, AST* rhs);
AST* assign(AST* to, AST* expr);
AST* call(char* name, list* args);
AST* num(int val);
AST* variable(char* name);
AST* access(char* name, AST* expr);
AST* ret(AST* expr);
AST* while_loop(AST*expr, AST* stmt);
AST* if_then(AST* expr, AST* if_stmt, AST* else_stmt);
AST* compound_stmt(list* decls, list* stmts);
AST* param(int type_spec, char* name, int ref);
AST* function(int ret_type, char* name, list* params, AST* body);
AST* var_decl(int type_spec, char* name, int size, int ref);

#endif

