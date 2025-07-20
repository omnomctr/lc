%{
#include "term.h"
#include <stdio.h>
#include "arena.h"

extern Arena *p_arena;

extern int yylex(void);

int yyerror(const char *s);

int yywrap(void);
%}
%union {
    const char *ident_name;
    Term *t;
}

%token <ident_name> IDENT
%type <t> expr
%type <t> appexpr

%%

input: /* empty */
     | input list
     ;

list: '\n'
    | expr '\n' { term_print($1); putchar('\n'); }
    | error '\n' { yyerrok; }
    ;

expr: IDENT { $$ = new_var(p_arena, $1); }
    | '\\' IDENT '.' expr { $$ = new_lam(p_arena, $2, $4); }
    | '(' appexpr ')' { $$ = $2; }
    ;

    /* the only way I found to have left associative applications */
appexpr: appexpr expr { $$ = new_app(p_arena, $1, $2); } 
       | expr { $$ = $1; }
       ;

%%

int yywrap(void)
{
    return 1;
}

int yyerror(const char *s)
{
    fprintf(stderr, "error: %s\n", s);
    return 0;
}
