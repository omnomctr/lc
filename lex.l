%{
#include "term.h"
#include "parse.tab.h"

extern Arena *p_arena;

%}

%%

[ \t] { ; } /* skip whitespace */
[a-zA-Z][a-zA-Z]* {
    yylval.ident_name = arena_strdup(p_arena, yytext);
    return IDENT;
}

[\n] { return '\n'; }
. { return *yytext; }
