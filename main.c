#include "term.h"
#include "parse.tab.h"
#include "arena.h"

Arena *p_arena;

int main(void) 
{
    yyparse();
    return 0;
}
