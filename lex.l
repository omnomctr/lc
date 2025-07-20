%{
#include "term.h"
#include "parse.tab.h"
#include <string.h>

%}

%%

[ \t] { ; } /* skip whitespace */
[a-zA-Z][a-zA-Z]* {
    // the strdup is required. it must be free'd in the parser stage
    yylval.ident_name = strdup(yytext);
    return IDENT;
}
":=" { return SET; }
[\n] { return '\n'; }
. { return *yytext; }
