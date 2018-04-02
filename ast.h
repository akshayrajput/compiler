#ifndef AST_H
#define AST_H

enum {
    BE_LT, BE_LTE,
    BE_GT, BE_GTE,
    BE_EQ, BE_NEQ,
    BE_ADD, BE_SUB,
    BE_MUL, BE_DIV,
    K_NUM, K_CALL,
    E_ASSIGN, VAR_NAME,
    ARRAY_ACCESS, RETURN,
    WHILE, IF_THEN,
    COMPOUND, PARAM,
    FUNCTION, VAR_DECL
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

