#ifndef PAIR_H_
#define PAIR_H_

#include <stdlib.h>

/**
 * @typedef keyT, valueT, const_keyT, const_valueT
 * typedef for the key and value elements in the pair, both regular and const versions.
 */
typedef void *keyT;
typedef const void *const_keyT;
typedef void *valueT;
typedef const void *const_valueT;

/**
 * @typedef pair_key_cpy, pair_value_cpy
 * Copy (dynamically) functions for the key and the value.
 */
typedef keyT (*pair_key_cpy) (const_keyT);
typedef valueT (*pair_value_cpy) (const_valueT);

/**
 * @typedef pair_key_cmp, pair_value_cmp
 * Compare functions for the key and the value. returns 1 if same, else - 0.
 */
typedef int (*pair_key_cmp) (const_keyT, const_keyT);
typedef int (*pair_value_cmp) (const_valueT, const_valueT);

/**
 * @typedef pair_key_free, pair_value_free
 * Free functions for the key and the value.
 */
typedef void (*pair_key_free) (keyT *);
typedef void (*pair_value_free) (valueT *);

/**
 * @struct pair - represent a pair '''{key: value}'''.
 * @param key, value - the key and value.
 * @param key_cpy, value_cpy - copy functions for key and value.
 * @param key_cmp, value_cmp - compare functions for key and value.
 * @param key_free, value_free - free functions for key and value.
 */
typedef struct pair {
    keyT key;
    valueT value;
    pair_key_cpy key_cpy;
    pair_value_cpy value_cpy;
    pair_key_cmp key_cmp;
    pair_value_cmp value_cmp;
    pair_key_free key_free;
    pair_value_free value_free;
} pair;

/**
 * Allocates dynamically a new pair.
 * @param key, value - the key and value.
 * @param key_cpy, value_cpy - copy functions for key and value.
 * @param key_cmp, value_cmp - compare functions for key and value.
 * @param key_free, value_free - free functions for key and value.
 * @return dynamically allocated pair.
 */
pair *pair_alloc (
    const_keyT key, const_valueT value,
    pair_key_cpy key_cpy, pair_value_cpy value_cpy,
    pair_key_cmp key_cmp, pair_value_cmp value_cmp,
    pair_key_free key_free, pair_value_free value_free);

/**
 * Creates a new (dynamically allocated) copy of the given old_pair.
 * @param old_pair old_pair to be copied.
 * @return new dynamically allocated old_pair if succeeded, NULL otherwise.
 */
void *pair_copy (const void *p);

/**
 * Compares two pairs
 * @param pair1 first pair
 * @param pair2 second pair
 * @return 1 if pairs are equal on key and value, 0 else
 */
int pair_cmp(const void *p1, const void *p2);

/**
 * This function frees a pair and everything it allocated dynamically.
 * @param p_pair pointer to dynamically allocated pair to be freed.
 */
void pair_free (void **p);

#endif //PAIR_H_
