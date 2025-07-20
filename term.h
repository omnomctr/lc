#ifndef TERM_HEADER__
#define TERM_HEADER__

#include "arena.h"
#include <stdbool.h>

typedef struct Term Term;
struct Term {
    enum {
        TK_VAR, /* variable */
        TK_LAM, /* lambda */
        TK_APP, /* application */
    } kind;
    union {
        const char *var;
        struct {
            const char *arg;
            Term *body;
        } lam;
        struct {
            Term *lhs;
            Term *rhs;
        } app;
    } as;
};

Term *new_var(Arena *a, const char *s);
Term *new_lam(Arena *a, const char *arg, Term *body);
Term *new_app(Arena *a, Term *lhs, Term *rhs);

void term_print(Term *t);
Term *term_copy(Arena *a, Term *t);

// evaluate step, creating a new term tree each time.
Term *eval_step(Arena *a, Term *t, bool *stabilised);

// evalutate, printing each step until
// the term is stabilized
void eval(Term *t);

#endif
