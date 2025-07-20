#include "term.h"
#include "parse.tab.h"
#include "arena.h"

Arena *p_arena;

int main(void) 
{
    p_arena = arena_new(1024);
    arena_clear(p_arena);

    yyparse();
    return 0;
}
