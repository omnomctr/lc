#include "term.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

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

/// TODO: write a garbage collector so we don't need to constantly create new arenas and copy every item every time (bad)


static Term *_replace(Arena *a, Term *t, const char *name, Term *with)
{
    switch (t->kind) {
        case TK_VAR: {
            if (!strcmp(t->as.var, name))
                return with;
            else
                return term_copy(a, t);
        } break;
        case TK_LAM: {
            if (!strcmp(t->as.lam.arg, name))
                return term_copy(a, t);
            else return new_lam(a, t->as.lam.arg, _replace(a, t->as.lam.body, name, with));
        } break;
        case TK_APP: {
            return new_app(a,
                    _replace(a, t->as.app.lhs, name, with),
                    _replace(a, t->as.app.rhs, name, with));
        } break;
    }

    assert(0 && "infallible");
    return NULL;
}

Term *eval_step(Arena *a, Term *t, bool *stabilised)
{
    switch (t->kind) {
        case TK_VAR: {
            return term_copy(a, t);
        } break;
        case TK_LAM: {
            return new_lam(a, arena_strdup(a, t->as.lam.arg), eval_step(a, t->as.lam.body, stabilised));
        } break;
        case TK_APP: {
            if (t->as.app.lhs->kind == TK_LAM) {
                *stabilised = false;
                return _replace(a, t->as.app.lhs->as.lam.body, t->as.app.lhs->as.lam.arg, term_copy(a, t->as.app.rhs));
            } else {
                Term *lhs_ = eval_step(a, t->as.app.lhs, stabilised);
                if (*stabilised == false) {
                    return new_app(a, lhs_, term_copy(a, t->as.app.rhs));
                }
                Term *rhs_ = eval_step(a, t->as.app.rhs, stabilised);
                return new_app(a, lhs_, rhs_);
            }
        } break;
    }

    assert(0 && "infallible");
    return NULL;
}

Term *term_copy(Arena *a, Term *t)
{
    switch (t->kind) {
        case TK_VAR: {
            return new_var(a, arena_strdup(a, t->as.var));
        } break;
        case TK_LAM: {
            return new_lam(a, t->as.lam.arg, t->as.lam.body);
        } break;
        case TK_APP: {
            return new_app(a, t->as.app.lhs, t->as.app.rhs);
        } break;
    }

    assert(0 && "infallible");
    return NULL;
}

void eval(Term *t)
{
    bool stabilised = false;
    size_t iterations = 1;

    Arena *prev = NULL;
    while (!stabilised) {
        Arena *a = arena_new(1024);
        stabilised = true;
        t = eval_step(a, t, &stabilised);
        printf("iteration %zu: ", iterations++);
        term_print(t);
        putchar('\n');
        if (prev) arena_destroy(prev);
        a = prev;
    }
}
