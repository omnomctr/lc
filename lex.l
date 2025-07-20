%{
#include "term.h"
#include "parse.tab.h"
#include <string.h>

%}

%%

#[^\n]* { ; } /* skip past # till next line for comments */
[ \t] { ; } /* skip whitespace */
[a-zA-Z0-9][a-zA-Z0-9]* {
    // the strdup is required. it must be free'd in the parser stage
    yylval.ident_name = strdup(yytext);
    return IDENT;
}
":=" { return SET; }
[\n] { return '\n'; }
. { return *yytext; }
