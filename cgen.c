#include "cgen.h"
#include "ast.h"
#include "list.h"
#include "utils.h"
#include "type.h"
#include <stdio.h>

static void codegen_(AST*);
static void codegen_bexpr(AST*);
static void codegen_assign(AST*);
static void codegen_array_access(AST*);
static void codegen_variable(AST*);
static void codegen_ret(AST*);
static void codegen_while(AST*);
static void codegen_if_then(AST*);
static void codegen_compound(AST*);
static void codegen_param(AST*);
static void codegen_function(AST*);
static void codegen_var_decl(AST*);

void codegen(list* program)
{
    for (struct node* tmp = program->head; tmp != NULL; tmp = tmp->next)
        codegen_(tmp->data);
}

static void codegen_(AST* node)
{
    switch (node->kind_)
    {
        case K_NUM:
        {   char buf[15];
            sprintf(buf,"%d", node->d.val_);
            emit3("mov", "eax", buf);
        }   break;
        // FALL THROUGH           
        case BE_LT:
        case BE_LTE:
        case BE_GT:
        case BE_GTE:
        case BE_EQ:
        case BE_NEQ:
        case BE_ADD:
        case BE_SUB:
        case BE_MUL:
        case BE_DIV:
            codegen_bexpr(node);
            break;
        
        case K_CALL:
            codegen(node->d.call.args_);
            break;
                    
        case E_ASSIGN:  
            codegen_assign(node);
            break;
        
        case ARRAY_ACCESS:  
            codegen_array_access(node);
            break;
            
        case VAR_NAME:
            codegen_variable(node);
            break;
       
        case RETURN:
            codegen_ret(node);
            break;
                    
        case WHILE: 
            codegen_while(node);
            break;
                    
        case IF_THEN:
            codegen_if_then(node);
            break;
    
        case COMPOUND:
            codegen_compound(node);
            break;
            
        case PARAM:
            codegen_param(node);
            break;
                 
        case FUNCTION:  
            codegen_function(node);
            break;
              
        case VAR_DECL:  
            codegen_var_decl(node);
            break;
        
        default:
            fprintf(stderr,"unknown node kind\n");
            break;
    }
}

static void codegen_bexpr(AST* node)
{
    codegen_(node->d.be.lhs_);
    emit2("push","eax");
    codegen_(node->d.be.rhs_);
    emit3("mov","ebx","eax");
    emit2("pop","eax");
    
    
    switch(node->kind_)
    {
    case BE_LT:
        emit3("cmp","eax","ebx");
        emit2("setl","esi");
        break;
    case BE_LTE:
        emit3("cmp","eax","ebx");
        emit2("setle","esi");
        break;
    case BE_GT:
        emit3("cmp","eax","ebx");
        emit2("setg","esi");
        break;
    case BE_GTE:
        emit3("cmp","eax","ebx");
        emit2("setge","esi");
        break;
    case BE_EQ:
        emit3("cmp","eax","ebx");
        emit2("sete","esi");
        break;
    case BE_NEQ:
        emit3("cmp","eax","ebx");
        emit2("setne","esi");
        break;
    case BE_ADD:
        emit3("add","eax","ebx");
        break;
    case BE_SUB:
        emit3("sub","eax","ebx");
        break;
    case BE_MUL:
        emit3("imul","eax","ebx");
        break;
    case BE_DIV:
        emit3("idiv","eax","ebx");
        break;
    default:
        fprintf(stderr,"unknown binary operator\n");
        break;
    }
}

static void codegen_assign(AST* node)
{
    codegen_(node->d.assign.expr_);
    codegen_(node->d.assign.to_);
    emit1("assign");
}

static void codegen_array_access(AST* node)
{
    codegen_(node->d.access.expr_);
    emit3("imul","eax","4");
    emit1("access");
}
//optimize this
static void codegen_variable(AST* node)
{
    
}

static void codegen_ret(AST* node)
{
    if(node->d.ret.expr_ != NULL)
    {
        codegen_(node->d.ret.expr_);
    }
    emit2("jmp",".exit");
}

static void codegen_while(AST* node)
{
    char* begin = new_label();
    char* end = new_label();
    emit_label(begin);
    codegen_(node->d.while_loop.expr_);
    emit2("jz",end);
    codegen_(node->d.while_loop.stmt_);
    emit2("jmp", begin);
    emit_label(end);    
}

static void codegen_if_then(AST* node)
{
    if(node->d.if_then.else_stmt_ == NULL)
    {
        char* end = new_label();
        codegen_(node->d.if_then.expr_);
        emit2("jz",end);
        codegen_(node->d.if_then.if_stmt_);
        emit_label(end);
    }
    else
    {
        char* not_if = new_label();
        char* end_if = new_label(); 
        codegen_(node->d.if_then.expr_);
        emit2("jz",not_if);
        codegen_(node->d.if_then.if_stmt_);
        emit2("jmp",end_if);
        emit_label(not_if);
        codegen_(node->d.if_then.else_stmt_);
        emit_label(end_if);
    }
}

static void codegen_compound(AST* node)
{
    codegen(node->d.compound.decls_);
    codegen(node->d.compound.stmts_);
}
//optimize this
static void codegen_param(AST* node)
{
        emit3("mov","esi","edi");
        emit2("push","esi");
}

static void codegen_function(AST* node)
{
    emit_label(node->d.function.name_);
    
    //prologue
    emit2("push", "ebp");
	emit3("mov", "ebp", "esp");
	emit3("sub", "esp", "0");
	emit2("push", "ebx");
	emit2("push", "esi");
	emit2("push", "edi");
	
    codegen(node->d.function.params_);
    codegen_(node->d.function.body_);
    
    //epilogue
    emit_label("exit");
    emit2("pop","edi");
    emit2("pop","esi");
    emit2("pop","ebx");
    emit3("add", "esp", "0");
    emit3("mov", "esp", "ebp");
    emit2("pop", "ebp");
    emit1("ret");
}

//optimize this
static void codegen_var_decl(AST* node)
{
    if(node->d.var_decl.type_spec_ != TYPE_INT)
    {
        emit3("sub","esp","4");
    }
    else
    {
        emit3("sub","esp","0");
    }
}
