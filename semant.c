#include "semant.h"
#include "type.h"
#include "symtab.h"
#include "ast.h"
#include <stdio.h>

static void semant(list* program, struct symtab* M, struct symtab* V);
static void semant_(AST* node, struct symtab* M, struct symtab* V);

void analyze(list* program)
{
   struct symtab* M = enterscope(NULL);
   struct symtab* V = enterscope(NULL);
   semant(program, M, V);
}

void semant(list* l, struct symtab* M, struct symtab* V)
{
    for (struct node* tmp = l->head; tmp != NULL; tmp = tmp->next)
        semant_(tmp->data, M, V);
}

static void semant_error(char* msg, AST* node)
{
    fprintf(stderr, "error:%d: %s\n",node->lineno_, msg);
}

static void set_type(AST* node, int t)
{
    node->type_ = t;
}

static void set_symtab(AST* node, struct symtab* M, struct symtab* V)
{
    node->M = M;
    node->V = V;
}

static void semant_bexpr(AST* node, char* s, struct symtab* M, struct symtab* V)
{
    semant_(node->d.be.lhs_, M, V);
    semant_(node->d.be.rhs_, M, V);
    set_symtab(node, M, V);
    
    if(node->d.be.lhs_->type_ == TYPE_INT && node->d.be.rhs_->type_ == TYPE_INT)
    {
        set_type(node,TYPE_INT);
    }
    else
    {
        char ar[100];
        sprintf(ar,"Type to %s should be integer",s);
        semant_error(ar,node);
        set_type(node,TYPE_ERR);   
    }
}

static void semant_assign(AST* node, struct symtab* M, struct symtab* V)
{
    set_symtab(node, M, V);
    semant_(node->d.assign.to_, M, V);
    semant_(node->d.assign.expr_, M, V);
    
    if(node->d.assign.expr_->type_ != TYPE_INT || node->d.assign.to_->type_ != TYPE_INT)
    {
        char ar[100];
        sprintf(ar, "Mismatch type, Cannot assign %s to %s",
            node->d.assign.expr_->type_ == TYPE_INT ? "int": "void",
            node->d.assign.to_->type_ == TYPE_INT ? "int": "void");
        semant_error(ar, node);
        set_type(node,TYPE_ERR);
    }
    else
    {
        set_type(node, TYPE_INT);
    }
}

static void semant_array(AST* node, struct symtab* M, struct symtab* V)
{
    set_symtab(node, M, V);
    semant_(node->d.access.expr_, M, V);
    
    if(node->d.access.expr_->type_ != TYPE_INT)
    {
        semant_error("Array Index should be of integer type", node);
        set_type(node,TYPE_ERR);
    }
    else
    {
        set_type(node,TYPE_INT);
    }
}

static void semant_variable(AST* node, struct symtab* M, struct symtab* V)
{
    set_symtab(node, M, V);
    
    if(lookup(V, node->d.variable.name_) == NULL)
    {
        char ar[100];
        sprintf(ar,"Undefined variable %s",node->d.variable.name_);
        semant_error(ar, node);
    }
    else
    {
        set_type(node, TYPE_INT);
    }
}

static void semant_ret(AST* node, struct symtab* M, struct symtab* V)
{
    set_symtab(node, M, V);
    
    if(node->d.ret.expr_ != NULL)
    {
        semant_(node->d.ret.expr_, M, V);
    
        if(node->d.ret.expr_->type_ != TYPE_INT)
        {
            semant_error("return type should be int",node);
            set_type(node, TYPE_ERR);
        }
        else
        {
            set_type(node, TYPE_INT);
        }
    }
    else
    {
        set_type(node, TYPE_VOID);
    }
}

static void semant_while(AST* node, struct symtab* M, struct symtab* V)
{
    set_symtab(node, M, V);
    semant_(node->d.while_loop.expr_, M, V);
    semant_(node->d.while_loop.stmt_, M, V);
    
    if(node->d.while_loop.expr_->type_ != TYPE_INT)
    {
        semant_error("Predicate to while should be of type int",node);
    }
}

static void semant_if_then(AST* node, struct symtab* M, struct symtab* V)
{
    set_symtab(node, M, V);   
    semant_(node->d.if_then.expr_, M, V);
    semant_(node->d.if_then.if_stmt_, M, V);
    
    if(node->d.if_then.else_stmt_ != NULL)
        semant_(node->d.if_then.else_stmt_, M, V);
    
    if(node->d.if_then.expr_->type_ != TYPE_INT)
    {
        set_type(node, TYPE_ERR);
        semant_error("Predicate to if should be int",node);
    }
    else
    {
        set_type(node, TYPE_INT);
    }
}

static void semant_compound(AST* node, struct symtab* M, struct symtab* V)
{
    V = enterscope(V);
    set_symtab(node, M, V);
    semant(node->d.compound.decls_, M, V);
    semant(node->d.compound.stmts_, M, V);
    V = exitscope(V);
}

static void semant_param(AST* node, struct symtab* M, struct symtab* V)
{
    V = add_symbol(V, node->d.param.name_, node);

    if (node->d.param.type_spec_ != TYPE_VOID)
        set_type(node, TYPE_INT);

    else
    {
        semant_error("Invalid parameter type", node);
        set_type(node,TYPE_ERR);
    }
}

static void semant_function(AST* node, struct symtab* M, struct symtab* V)
{
    M = add_symbol(M, node->d.function.name_, node);
    V = enterscope(V);
    set_symtab(node, M, V);

    // check parameters
    semant(node->d.function.params_, M, V);
    // check body
    semant_(node->d.function.body_, M, V);
    V = exitscope(V);

    /*if(node->d.function.ret_type_ != node->d.function.body_->type_)
    {
        semant_error("Body does not conform to function's return type",node);
        set_type(node,TYPE_ERR);
    }

    else*/
    {
        set_type(node, node->d.function.body_->type_);
    }
}

static void semant_var_decl(AST* node, struct symtab* M, struct symtab* V)
{
    V = add_symbol(V, node->d.var_decl.name_, node);
    set_symtab(node, M, V);
    if(node->d.var_decl.type_spec_ != TYPE_INT)
    {
        semant_error("Type of a variable should be int", node);
        set_type(node, TYPE_ERR);
    }
    else
    {
        set_type(node, TYPE_INT);
    }
}

static void semant_(AST* node, struct symtab* M, struct symtab* V)
{
    switch (node->kind_)
    {
        case K_NUM:
            set_type(node, TYPE_INT);
            set_symtab(node, M, V);
            break;
                    
        case BE_LT:
            semant_bexpr(node, "<", M, V);
            break;
                    
        case BE_LTE:
            semant_bexpr(node, "<=", M, V);
            break;
                    
        case BE_GT:
            semant_bexpr(node, ">", M, V);
            break;

        case BE_GTE:
            semant_bexpr(node, ">=", M, V);
            break;

        case BE_EQ:
            semant_bexpr(node, "==", M, V);
            break;

        case BE_NEQ:
            semant_bexpr(node, "!=", M, V);
            break;

        case BE_ADD:
            semant_bexpr(node, "+", M, V); 
            break;

        case BE_SUB:
            semant_bexpr(node, "-", M, V);
            break;

        case BE_MUL:
            semant_bexpr(node, "*", M, V);
            break;

        case BE_DIV:
            semant_bexpr(node, "/", M, V);
            break;
        
        case K_CALL:
            set_symtab(node, M, V);
            semant(node->d.call.args_, M, V);
            break;
                    
        case E_ASSIGN:  
            semant_assign(node, M, V);
            break;
        
        case ARRAY_ACCESS:  
            semant_array(node, M, V);
            break;
            
        case VAR_NAME:
            semant_variable(node, M, V);
            break;
       
        case RETURN:
            semant_ret(node, M, V);
            break;
                    
        case WHILE: 
            semant_while(node, M, V);
            break;
                    
        case IF_THEN:
            semant_if_then(node, M, V);
            break;
    
        case COMPOUND:
            semant_compound(node, M, V);
            break;
            
        case PARAM:
            semant_param(node, M, V);
            break;
                 
        case FUNCTION:  
            semant_function(node, M, V);
            break;
              
        case VAR_DECL:  
            semant_var_decl(node, M, V);
            break;
        
        default:
            fprintf(stderr,"unknown node kind\n");
            break;
    }
}

