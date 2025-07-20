// I'm currently using stb_ds. I might make my own hashmap
// at some point but idrc rn
#define STB_DS_IMPLEMENTATION
#include "vendored/stb_ds.h"

#include "constants.h"

struct { char *key; Term *value; } *constant_map = NULL;

void put_constant(char *key, Term *t)
{
    key = strdup(key);
    shput(constant_map, key, t);
}

Term *get_constant(char *key)
{
    return shget(constant_map, key);
}

void for_each_constant(void (*fn)(Term *))
{
    for (int i = 0; i < shlen(constant_map); i++)
        (*fn)(constant_map[i].value);
}
