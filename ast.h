#ifndef AST_H
#define AST_H

enum {
    TOKEN_BE_LT,
    TOKEN_BE_LTE,
    TOKEN_BE_GT,
    TOKEN_BE_GTE,
    TOKEN_BE_EQ,
    TOKEN_BE_NEQ,
    TOKEN_BE_ADD,
    TOKEN_BE_SUB,
    TOKEN_BE_MUL,
    TOKEN_BE_DIV,
    TOKEN_K_NUM,
    TOKEN_K_CALL,
    TOKEN_E_ASSIGN,
    TOKEN_VAR_NAME,
    TOKEN_ARRAY_ACCESS,
    TOKEN_RETURN,
    TOKEN_WHILE,
    TOKEN_IF_THEN,
    TOKEN_COMPOUND,
    TOKEN_PARAM,
    TOKEN_FUNCTION,
    TOKEN_VAR_DECL
} kind;

struct list; // forward declaration

typedef struct AST AST;
struct AST
{
    int lineno_;
    int kind_; // kind of the ast node
    int type_; // type of the ast node
    struct symtab* M; // to keep track of methods
    struct symtab* V; // to keep track of variables

    union {
        int val_;
        
        struct {
            AST* lhs_;
            AST* rhs_;
        } be;
        
        struct {
            char* name_;
            struct list* args_;
        } call;
        
        struct {
            AST* to_;
            AST* expr_;
        } assign;
        
        struct {
            char* name_;
        } variable;
        
        struct {
            char* name_;
            AST* expr_;
        } access;
        
        struct {
            AST* expr_;
        } ret;
        
        struct {
            AST* expr_;
            AST* stmt_;
        } while_loop;   
        
        struct {
            AST* expr_;
            AST* if_stmt_;
            AST* else_stmt_;
        } if_then;
        
        struct {
            struct list* decls_;
            struct list* stmts_;
        } compound;     
        
        struct {
            int type_spec_;
            char* name_;
            int ref_;
        } param;
        
        struct {
            int ret_type_;
            char* name_;
            struct list* params_;
            AST* body_;
        } function;
        
        struct {
            int type_spec_;
            char* name_;
            int size_;
            int ref_;
        } var_decl;
        
    } d;
};

#endif

