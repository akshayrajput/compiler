#include "ast_cons.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>

extern int yylineno;

static AST* make_ast(int lineno, int kind)
{
    AST* tmp = (AST*)malloc(sizeof *tmp);
    tmp->lineno_ = lineno;    
    tmp->kind_ = kind;
    return tmp;
}

static AST* make_binexpr(int lineno, int type, AST* lhs, AST* rhs)
{
    AST* tmp = make_ast(lineno, type);
    tmp->d.be.lhs_ = lhs;
    tmp->d.be.rhs_ = rhs;
    return tmp;
}

AST* lt(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_LT, lhs, rhs);
}

AST* lte(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_LTE, lhs, rhs);
}

AST* gt(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_GT, lhs, rhs);
}

AST* gte(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_GTE, lhs, rhs);
}

AST* eq(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_EQ, lhs, rhs);
}

AST* neq(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_NEQ, lhs, rhs);
}

AST* add(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_ADD, lhs, rhs);
}

AST* sub(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_SUB, lhs, rhs);
}

AST* mul(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_MUL, lhs, rhs);
}

AST* divide(AST* lhs, AST* rhs)
{
    return make_binexpr(yylineno, BE_DIV, lhs, rhs);
}

AST* call(char* name, list* args)
{
    AST* ast = make_ast(yylineno, K_CALL);
    //printf(name);
    ast->d.call.name_ = name;
    ast->d.call.args_ = args;
    return ast;
}

AST* assign(AST* to, AST* expr)
{
    AST* temp = make_ast(yylineno, E_ASSIGN);
    temp->d.assign.to_ = to;
    temp->d.assign.expr_ = expr;
    return temp;
}

AST* variable(char* name)
{
    AST* temp = make_ast(yylineno, VAR_NAME);
    temp->d.variable.name_ = name;
    return temp;
}

AST* num(int val)
{
    AST* ast = make_ast(yylineno, K_NUM);
    ast->d.val_ = val;
    return ast;
}

AST* access(char* name, AST* expr)
{
    AST* temp = make_ast(yylineno, ARRAY_ACCESS);
    temp->d.access.name_ = name;
    temp->d.access.expr_ = expr;
    return temp;
}

AST* ret(AST *expr)
{
    AST* temp = make_ast(yylineno, RETURN);
    temp->d.ret.expr_ = expr;
    return temp;
}

AST* while_loop(AST* expr, AST* stmt)
{
    AST* temp = make_ast(yylineno, WHILE);
    temp->d.while_loop.expr_ = expr;
    temp->d.while_loop.stmt_ = stmt;
    return temp;
}

AST* if_then(AST* expr, AST* if_stmt, AST* else_stmt)
{
    AST* temp = make_ast(yylineno, IF_THEN);
    temp->d.if_then.expr_ = expr;
    temp->d.if_then.if_stmt_ = if_stmt;
    temp->d.if_then.else_stmt_ = else_stmt;
    return temp;
}

AST* compound_stmt(list* decls, list* stmts)
{
    AST* temp = make_ast(yylineno, COMPOUND);
    temp->d.compound.decls_ = decls;
    temp->d.compound.stmts_ = stmts;
    return temp;
}

AST* param(int type_spec, char* name, int ref)
{
    AST* temp = make_ast(yylineno, PARAM);
    temp->d.param.type_spec_ = type_spec;
    temp->d.param.name_ = name;
    temp->d.param.ref_ = ref;
    return temp;
}

AST* function(int ret_type, char* name, list* params, AST* body)
{
    AST* temp = make_ast(yylineno, FUNCTION);
    temp->d.function.ret_type_ = ret_type;
    temp->d.function.name_ = name;
    temp->d.function.params_ = params;
    temp->d.function.body_ = body;
    return temp;
}

AST* var_decl(int type_spec, char* name, int size, int ref)
{
    AST* temp = make_ast(yylineno, VAR_DECL);
    temp->d.var_decl.type_spec_ = type_spec;
    temp->d.var_decl.name_ = name;
    temp->d.var_decl.size_ = size;
    temp->d.var_decl.ref_ = ref;
    return temp;
}
