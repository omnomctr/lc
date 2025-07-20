#include "arena.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

Arena *arena_new(size_t capacity)
{
    if (capacity == 0) capacity = ARENA_DEFAULT_SIZE;
    Arena *ret = malloc(sizeof(Arena) + sizeof(unit) * capacity);
    assert(ret && "buy more ram");

    *ret = (Arena) {
        .cursor = 0,
        .capacity = capacity,
        .next = NULL,
    };

    return ret;
}


void arena_destroy(Arena *a)
{
    if (a->next) arena_destroy(a->next);
    free(a);
}

void *arena_alloc(Arena *a, size_t size)
{
    if (a->capacity <= a->cursor + size) {
        if (a->next) return arena_alloc(a->next, size);
        else {
            a->next = arena_new(a->capacity + size);
            return arena_alloc(a->next, size);
        }
    } else {
        void *ptr = &a->pool[a->cursor];
        a->cursor += size;
        return ptr;
    }
}

/* clears the arena for reuse */
void arena_clear(Arena *a)
{
   a->cursor = 0;
   if (a->next) arena_clear(a->next);
}

char *arena_strdup(Arena *a, const char *s)
{
    size_t s_len = strlen(s);
    char *ret = arena_alloc(a, (s_len + 1) * sizeof(char));
    memcpy(ret, s, s_len);
    ret[s_len] = '\0';
    return ret;
}
