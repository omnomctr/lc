#include <string.h>
#include "arena.h"
#include <stdio.h>
#include "constants.h"

struct ConstantMapRecord {
    char *key;
    Term *value;
    struct ConstantMapRecord *next; /* nullable */
};

#define CONSTANT_MAP_LEN 1024
#define CM_MULTIPLIER 31

static struct ConstantMapRecord *_constant_map[CONSTANT_MAP_LEN] = {0};

static Arena *_cm_arena;

void constant_map_init(void)
{
    _cm_arena = arena_new(1024);
}

void constant_map_cleanup(void)
{
    arena_destroy(_cm_arena);
}

static size_t hash(const char *key)
{
    size_t h = 0;

    for (unsigned char *p = (unsigned char *)key; *p != '\0'; p++)
        h = CM_MULTIPLIER * h + *p;

    return h % CONSTANT_MAP_LEN;
}

void put_constant(char *key, Term *t)
{
    size_t h = hash(key);
    struct ConstantMapRecord **p = &_constant_map[h];
    
    while (*p != NULL && !!strcmp((*p)->key, key)) 
        p = &(*p)->next;

    if (!*p) {
        *p = arena_alloc(_cm_arena, sizeof(struct ConstantMapRecord));
        **p = (struct ConstantMapRecord) {
            .key = arena_strdup(_cm_arena, key),
            .value = t,
            .next = NULL,
        };
    } else {
        (*p)->value = t;
    }

}

Term *get_constant(char *key)
{
    size_t h = hash(key);
    struct ConstantMapRecord *p = _constant_map[h];
    while (p != NULL && !!strcmp(p->key, key))
            p = p->next;

    return p == NULL ? NULL : p->value;
}


void for_each_constant(void (*fn)(Term *))
{
    for (size_t i = 0; i < CONSTANT_MAP_LEN; i++)
        for (struct ConstantMapRecord *p = _constant_map[i]; p != NULL; p = p->next)
            (*fn)(p->value);
}
