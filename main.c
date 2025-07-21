#include "term.h"
#include "parse.tab.h"
#include <string.h>

int main(int argc, char **argv) 
{
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-hide-steps")) 
            hide_steps = true;
    }
    yyparse();
    return 0;
}
