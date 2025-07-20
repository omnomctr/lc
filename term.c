#include "term.h"
#include <stdio.h>

Term *new_var(Arena *a, const char *s)
{
    Term *ret = arena_alloc(a, sizeof(Term));
    ret->kind = TK_VAR;
    ret->as.var = s;

    return ret;
}

Term *new_lam(Arena *a, const char *arg, Term *body)
{
    Term *ret = arena_alloc(a, sizeof(Term));
    ret->kind = TK_LAM;
    ret->as.lam.arg = arg;
    ret->as.lam.body = body;

    return ret;
}

Term *new_app(Arena *a, Term *lhs, Term *rhs)
{
    Term *ret = arena_alloc(a, sizeof(Term));
    ret->kind = TK_APP;
    ret->as.app.lhs = lhs;
    ret->as.app.rhs = rhs;

    return ret;
}

void term_print(Term *t)
{
    switch (t->kind) {
        case TK_VAR: printf("%s", t->as.var); break;
        case TK_LAM: {
            printf("λ%s.", t->as.lam.arg);
            term_print(t->as.lam.body);
        } break;
        case TK_APP: {
            printf("(");
            term_print(t->as.app.lhs);
            printf(" ");
            term_print(t->as.app.rhs);
            printf(")");
        } break;
    }
}
