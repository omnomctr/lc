%{
#include "term.h"
#include "parse.tab.h"
#include <string.h>

extern unsigned int line_no;
%}

%%

#[^\n]* { ; } /* skip past # till next line for comments */
[ \t] { ; } /* skip whitespace */
[a-zA-Z][a-zA-Z0-9]*\??\'* {
    // the strdup is required. it must be free'd in the parser stage
    yylval.ident_name = strdup(yytext);
    return IDENT;
}
"\\" { return LAM; }
"λ" { return LAM; }
[0-9]+ {
    yylval.num = atol(yytext);
    return NUM;
}
":=" { return SET; }
[\n] { line_no++; /*return '\n';*/ }
. { return *yytext; }
