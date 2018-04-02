%{
#include "parser.tab.h"
%}

%option warn noyywrap noinput nounput yylineno

%%
"else" 		{return ELSE;}
"if" 		{return IF;}
"int" 		{return INT;}
"return" 	{return RETURN;}
"void" 		{return VOID;}
"while" 	{return WHILE;}
"+"		    {return ADD;}
"-"	    	{return SUB;}
"*"	    	{return MUL;}
"/"	    	{return DIV;}
"<"	    	{return LT;}
"<="		{return LTE;}
">" 		{return GT;}
">="		{return GTE;}
"=="		{return EQ;}
"!="		{return NEQ;}
"=" 		{return ASSIGN;}
";" 		{return SEMI;}
"," 		{return COMMA;}
"(" 		{return ROUND_OPEN;}
")" 		{return ROUND_CLOSE;}
"[" 		{return SQUARE_OPEN;}
"]" 		{return SQUARE_CLOSE;}
"{" 		{return CURLY_OPEN;}
"}" 		{return CURLY_CLOSE;}
[a-zA-Z]+	  {yylval.string = strdup(yytext); return ID;}
[0-9]+		  {yylval.integer = atoi(yytext); return NUM;}
"\n"        {++yylineno;}
[ \t\b]+    /* discard whitespace */
"//".*		  /* discard comments */
.           { fprintf(stderr, "unknown character: %c", yytext[0]); }
%%

