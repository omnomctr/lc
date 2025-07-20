%{
#include "term.h"
#include <stdio.h>
#include "arena.h"
#include "constants.h"

extern int yylex(void);
int yyerror(const char *s);
int yywrap(void);

%}
%union {
    char *ident_name;
    Term *t;
}

%token <ident_name> IDENT
%type <t> expr
%type <t> appexpr

%right SET

%%

input: /* empty */
     | input list
     ;

list: '\n'
    | expr '\n' { term_print($1); putchar('\n'); eval($1); }
    | IDENT SET expr { put_constant($1, $3); }
    | error '\n' { yyerrok; }
    ;

expr: IDENT { 
        Term *resolv = get_constant($1);
        $$ = resolv == NULL ? /* constant not found */ new_var($1) : resolv;
        free($1);
    }
    | '\\' IDENT '.' expr { $$ = new_lam($2, $4); free($2); }
    | '(' appexpr ')' { $$ = $2; }
    ;

    /* the only way I found to have left associative applications */
appexpr: appexpr expr { $$ = new_app($1, $2); } 
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
