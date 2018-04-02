%{
#include "ast.h"
#include "list.h"
#include "type.h"
#include "ast_cons.h"
#include <stdio.h>

extern int yylex();
void yyerror(char* err, ...);

extern list* program;
%}

%union {
    struct AST;
    struct list;

    struct AST* ast;
    struct list* list;
    char* string;
    int integer;
    int type;
};

%token VOID INT
%token ELSE IF RETURN 
%token WHILE
%token ADD SUB MUL DIV
%token LT LTE GT GTE EQ NEQ
%token ASSIGN
%token SEMI COMMA
%token ROUND_OPEN ROUND_CLOSE SQUARE_OPEN SQUARE_CLOSE CURLY_OPEN CURLY_CLOSE
%token ID NUM
%token NL


%type <list> program
%type <list> declaration_list
%type <ast> declaration
%type <ast> var_declaration
%type <type> type_specifier
%type <ast> fun_declaration
%type <list> params
%type <list> param_list
%type <ast> param
%type <ast> compound_stmt
%type <list> local_declarations
%type <list> statement_list
%type <ast> statement
%type <ast> expression_stmt
%type <ast> selection_stmt
%type <ast> iteration_stmt
%type <ast> return_stmt
%type <ast> expression
%type <ast> var
%type <ast> simple_expression
%type <ast> additive_expression
%type <ast> term
%type <ast> factor
%type <ast> call
%type <list> args
%type <string> ID
%type <integer> NUM
%type <type> VOID
%type <type> INT

%%
program:
      declaration_list 
        { program = $1; }
    ;

declaration_list:
      declaration_list declaration
        { $$ = append_list($1, single_list($2)); }
    | declaration
        { $$ = single_list($1); }
    ;

declaration:
      var_declaration
    | fun_declaration
    ;

var_declaration:
      type_specifier ID SEMI
        { $$ = var_decl($1, $2, 0, 0); }
    | type_specifier ID SQUARE_OPEN NUM SQUARE_CLOSE SEMI
        { $$ = var_decl($1, $2, $4, 1); }
    ;

type_specifier:
      INT
        { $$ = TYPE_INT; }
    | VOID
        { $$ = TYPE_VOID; }
    ;

fun_declaration:
      type_specifier ID ROUND_OPEN params ROUND_CLOSE compound_stmt
        { $$ = function($1, $2, $4, $6); }
    ;

params:
      param_list
    | /* empty */
        { $$ = nil_list(); }
    ;

param_list:
      param_list COMMA param
        { $$ = append_list($1, single_list($3)); }
    | param
        { $$ = single_list($1); }
    ;

param:
      type_specifier ID
        { $$ = param($1, $2, 0); }
    | type_specifier ID SQUARE_OPEN SQUARE_CLOSE
        { $$ = param($1, $2, 1); }
    ;

compound_stmt:
      CURLY_OPEN local_declarations statement_list CURLY_CLOSE
        { $$ = compound_stmt($2, $3); }
    ;

local_declarations:
      local_declarations var_declaration
        { $$ = append_list($1, single_list($2)); }
    | /* empty */
        { $$ = nil_list(); }
    ;

statement_list:
      statement_list statement
        { $$ = append_list($1, single_list($2)); }
    | /* empty */
        { $$  = nil_list(); }
    ;

statement:
      expression_stmt
    | compound_stmt
    | selection_stmt
    | iteration_stmt
    | return_stmt
    ;

expression_stmt:
      expression SEMI
    | SEMI
        { $$ = NULL; }
    ;

selection_stmt:
      IF ROUND_OPEN expression ROUND_CLOSE statement
        { $$ = if_then($3, $5, NULL); }
    | IF ROUND_OPEN expression ROUND_CLOSE statement ELSE statement
        { $$ = if_then($3, $5, $7); }
    ;

iteration_stmt:
      WHILE ROUND_OPEN expression ROUND_CLOSE statement
        { $$ = while_loop($3, $5); }
    ;

return_stmt:
      RETURN SEMI
        { $$ = ret(NULL); }
    | RETURN expression SEMI
        { $$ = ret($2); }
    ;

expression:
      var ASSIGN expression
        { $$ = assign($1, $3); }    // save yytext's value
    | simple_expression
    ;

var: 
      ID
        { $$ = variable($1); }
    | ID SQUARE_OPEN expression SQUARE_CLOSE
        { $$ = access($1, $3); }
    ;

simple_expression:
      additive_expression LT additive_expression
        { $$ = lt($1, $3); }
    | additive_expression LTE additive_expression
        { $$ = lte($1, $3); }
    | additive_expression GT additive_expression
        { $$ = gt($1, $3); }
    | additive_expression GTE additive_expression
        { $$ = gte($1, $3); }
    | additive_expression EQ additive_expression
        { $$ = eq($1, $3); }
    | additive_expression NEQ additive_expression
        { $$ = neq($1, $3); }
    | additive_expression
    ;

additive_expression:
      additive_expression ADD term 
        { $$ = add($1, $3); }
    | additive_expression SUB term
        { $$ = sub($1, $3); }
    | term 
    ;

term: 
      term MUL factor 
        { $$ = mul($1, $3); }
    | term DIV factor 
        { $$ = divide($1, $3); }
    | factor
    ;

factor: 
      NUM
        { $$ = num($1); }
    | ROUND_OPEN expression ROUND_CLOSE
        { $$ = $2; }
    | var
    | call
    ;

call:
       ID ROUND_OPEN args ROUND_CLOSE
        { $$ = call($1, $3); }  //save yytext's value
    ;

args:
      args COMMA expression
        { $$ = append_list($1, single_list($3)); }
    | expression
        { $$ = single_list($1); }
    | /* empty */
        { $$ = nil_list(); }
    ;

%%

void yyerror(char* err, ...)
{
	fprintf(stderr, "%s\n", err);
}

