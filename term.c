#include "term.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "constants.h"

/* gc stuff */
static struct {
    size_t live_count;
    Term *term_list;
} _gc_state = {
    .live_count = 0,
    .term_list = NULL,
};

static Term *_term_new_generic(void) 
{
    Term *ret = malloc(sizeof(Term));
    assert(ret && "buy more ram");
    ret->_gc_next = _gc_state.term_list;
    ret->_gc_mark = false;
    _gc_state.term_list = ret;
    _gc_state.live_count++;

    return ret;
}

static void term_free(Term *t) 
{
    switch (t->kind) {
        case TK_VAR: free(t->as.var); break;
        case TK_LAM: free(t->as.lam.arg); break;
        case TK_APP: break;
    }
    free(t);
}

void _gc_mark(Term *t)
{
    if (t->_gc_mark) return;
    t->_gc_mark = true;
    switch (t->kind) {
        case TK_VAR: break;
        case TK_LAM: _gc_mark(t->as.lam.body); break;
        case TK_APP:
            _gc_mark(t->as.app.lhs);
            _gc_mark(t->as.app.rhs);
            break;
    }
}

void _gc_sweep(void)
{
    Term **t = &_gc_state.term_list;
    while (*t) {
        if (!(*t)->_gc_mark) {
            Term *unreachable = *t;
            *t = unreachable->_gc_next;
            term_free(unreachable);
            _gc_state.live_count--;
        } else {
            /* reset the object from marking */
            (*t)->_gc_mark = false;
            t = &(*t)->_gc_next;
        }
    }
}

void _run_gc(char dummy, ...) 
{
    va_list ap;
    va_start(ap, dummy);
    Term *to_mark;
    while ((to_mark = va_arg(ap, Term *)) != NULL)
        _gc_mark(to_mark);

    va_end(ap);

    for_each_constant(_gc_mark);
    _gc_sweep();
}

Term *new_var(char *s)
{
    Term *ret = _term_new_generic();
    ret->kind = TK_VAR;
    ret->as.var = strdup(s);

    return ret;
}

Term *new_lam(char *arg, Term *body)
{
    Term *ret = _term_new_generic();
    ret->kind = TK_LAM;
    ret->as.lam.arg = strdup(arg);
    ret->as.lam.body = body;

    return ret;
}

Term *new_app(Term *lhs, Term *rhs)
{
    Term *ret = _term_new_generic();
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

static Term *_replace(Term *t, const char *name, Term *with)
{
    switch (t->kind) {
        case TK_VAR: {
            if (!strcmp(t->as.var, name)) {
                return with;
            } else
                return t;
        } break;
        case TK_LAM: {
            if (!strcmp(t->as.lam.arg, name))
                return t;
            else return new_lam(t->as.lam.arg, _replace(t->as.lam.body, name, with));
        } break;
        case TK_APP: {
            return new_app(
                    _replace(t->as.app.lhs, name, with),
                    _replace(t->as.app.rhs, name, with));
        } break;
    }

    assert(0 && "infallible");
    return NULL;
}

Term *eval_step(Term *t, bool *stabilised)
{
    switch (t->kind) {
        case TK_VAR: {
            return t;
        } break;
        case TK_LAM: {
            return new_lam(t->as.lam.arg, eval_step(t->as.lam.body, stabilised));
        } break;
        case TK_APP: {
            if (t->as.app.lhs->kind == TK_LAM) {
                *stabilised = false;
                return _replace(t->as.app.lhs->as.lam.body, t->as.app.lhs->as.lam.arg, t->as.app.rhs);
            } else {
                Term *lhs_ = eval_step(t->as.app.lhs, stabilised);
                if (*stabilised == false) {
                    return new_app(lhs_, t->as.app.rhs);
                }
                Term *rhs_ = eval_step(t->as.app.rhs, stabilised);
                return new_app(lhs_, rhs_);
            }
        } break;
    }

    assert(0 && "infallible");
    return NULL;
}

Term *term_copy(Term *t)
{
    switch (t->kind) {
        case TK_VAR: {
            return new_var(t->as.var);
        } break;
        case TK_LAM: {
            return new_lam(t->as.lam.arg, t->as.lam.body);
        } break;
        case TK_APP: {
            return new_app(t->as.app.lhs, t->as.app.rhs);
        } break;
    }

    assert(0 && "infallible");
    return NULL;
}

void eval(Term *t)
{
    bool stabilised = false;
    size_t iterations = 0;

    printf("%zu: ", iterations++); 
    term_print(t); putchar('\n');
    for (;;) {
        stabilised = true;
        t = eval_step(t, &stabilised);
        if (stabilised) { printf("done\n"); break; }
        printf("%zu: ", iterations++);
        term_print(t); putchar('\n');
        run_gc(t);
    }
}
