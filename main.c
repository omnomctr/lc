#include "term.h"
#include "parse.tab.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin; // used by lex

int main(int argc, char **argv) 
{
    yyin = stdin;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-hide-steps")) 
            hide_steps = true;

        if (!strcmp(argv[i], "-run")) {
            // ran with a shebang
            if (yyin != stdin) {
                fprintf(stderr, "error: you may only run one file\n");
                exit(1);
            }
            const char *file_path = argv[i+1];
            FILE *f = fopen(file_path, "r");
            if (!f) {
                fprintf(stderr, "error opening file %s\n", file_path);
                exit(1);
            }
            yyin = f;
        }

    }
    yyparse();

    if (yyin != stdin) fclose(yyin);
    return 0;
}
