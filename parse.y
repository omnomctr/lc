%{
#include "term.h"
#include <stdio.h>
#include "constants.h"

extern int yylex(void);
int yyerror(const char *s);
int yywrap(void);

extern int line_no;

%}
%union {
    char *ident_name;
    Term *t;
    long num;
}

%token <ident_name> IDENT
%token <num> NUM 

%type <t> expr
%type <t> application
%type <t> item

%right SET

%%

input: /* empty */
     | input list
     ;

list: '\n'
    | expr '\n' { eval($1); }
    | IDENT SET expr '\n' { put_constant($1, $3); }
    | error '\n' { yyerrok; }
    ;

/* https://en.wikipedia.org/wiki/Lambda_calculus_definition#Syntax_definition_in_BNF */
expr: application { $$ =$1; } 
    | '\\' IDENT '.' expr { $$ = new_lam($2, $4); free($2); }
    ;

application: application item { $$ = new_app($1, $2); }
           | item { $$ = $1; }
           ;


item: IDENT {
        Term *resolv = get_constant($1);
        $$ = resolv == NULL ? /* constant not found */ new_var($1) : resolv;
        free($1);
    }
    | NUM { $$ = new_church_numeral($1); }
    | '(' expr ')' { $$ = $2; }
    ;
%%

int yywrap(void)
{
    return 1;
}

int yyerror(const char *s)
{
    fprintf(stderr, "error: %s, line %d\n", s, line_no);
    return 0;
}
