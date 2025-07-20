#ifndef CONSTANTS_HEADER__
#define CONSTANTS_HEADER__

/* deals with the resolution of constants created with the ':=' construct */

#include "term.h"
#include <stddef.h>

void put_constant(char *key, Term *t);
Term *get_constant(char *key);

// we only have this so I can mark the terms 
// held in the constant map w/o circular dependencies
void for_each_constant(void (*fn)(Term *));

#endif
