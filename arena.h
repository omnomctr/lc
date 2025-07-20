/* I took this from my other repository deeprose3 https://github.com/omnomctr/deeprose3/blob/main/arena.h teehee */
#ifndef ARENA_HEADER_
#define ARENA_HEADER_

#include <stddef.h>

/* The arena is implemented as a super simple bump allocator, 
 * meaning no reallocs, no induvidual frees. 
 * However, when an allocation size requires more than 
 * the arena's capacity, to avoid expensive reallocs, 
 * I made a linked list sort of thingy, attaching arenas together */


typedef char unit;
_Static_assert(sizeof(unit) == 1, "");

#define ARENA_DEFAULT_SIZE 4096

typedef struct Arena Arena;
struct Arena {
    size_t cursor;
    size_t capacity;
    Arena *next; /* nullable */
    unit pool[]; /* flexible array member */
};

Arena *arena_new(size_t capacity);
void arena_destroy(Arena *a);
void *arena_alloc(Arena *a, size_t size);
void arena_clear(Arena *a);

char *arena_strdup(Arena *a, const char *s);

#endif
