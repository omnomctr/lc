#ifndef TERM_HEADER__
#define TERM_HEADER__

#include <stdbool.h>

typedef struct Term Term;
struct Term {
    enum {
        TK_VAR, /* variable */
        TK_LAM, /* lambda */
        TK_APP, /* application */
    } kind;
    union {
        char *var;
        struct {
            char *arg;
            Term *body;
        } lam;
        struct {
            Term *lhs;
            Term *rhs;
        } app;
    } as;

    Term *_gc_next; /* nullable */
    bool _gc_mark;
};

extern bool hide_steps;

Term *new_var(char *s);
Term *new_lam(char *arg, Term *body);
Term *new_app(Term *lhs, Term *rhs);

void term_print(Term *t);
Term *term_copy(Term *t);

Term *new_church_numeral(long n);

struct BoundList {
    char *name;
    struct BoundList *next; /* nullable */
};

// evaluate step, creating a new term tree each time.
Term *eval_step(Term *t, bool *stabilised, struct BoundList *bound);

// evalutate, printing each step until
// the term is stabilized
void eval(Term *t);

#define run_gc(...) \
    /* variadic functions must have a
     * name parameter until c23 */ \
    _run_gc('d' __VA_OPT__(,) __VA_ARGS__, NULL);

void _run_gc(char dummy, ...);

#endif
