#include "term.h"
#include "parse.tab.h"

int main(void) 
{
    yyparse();
    return 0;
}
