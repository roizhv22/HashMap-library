#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdlib.h>
#include "vector.h"
#include "pair.h"

/**
 * @def HASH_MAP_INITIAL_CAP
 * The initial capacity of the hash map.
 * It means, the initial number of <b> vectors </b> the hash map has.
 */
#define HASH_MAP_INITIAL_CAP 16UL

/**
 * @def HASH_MAP_GROWTH_FACTOR
 * The growth factor of the hash map.
 * Example: hash_map(size=16),
 *          hash_map.add([16 elements]) ->
 *          hash_map(size=32)
 */
#define HASH_MAP_GROWTH_FACTOR 2UL

/**
 * @def HASH_MAP_MIN_LOAD_FACTOR
 * The minimal load factor the hash map can be in.
 * Example: if the hash_map capacity is 16,
 * and it has 4 elements in it (size is 4),
 * if an element is erased, the load factor drops below 0.25,
 * so the hash map should be minimized (to 8 vectors).
 */
#define HASH_MAP_MIN_LOAD_FACTOR 0.25

/**
 * @def HASH_MAP_MAX_LOAD_FACTOR
 * The maximal load factor the hash map can be in.
 * Example: if the hash_map capacity is 16,
 * and it has 12 elements in it (size is 12),
 * if another element is added, the load factor goes above 0.75,
 * so the hash map should be extended (to 32 vectors).
 */
#define HASH_MAP_MAX_LOAD_FACTOR 0.75

/**
 * @typedef hash_func
 * This type of function receives a keyT and returns
 * a representational number of it.
 * Example: lets say we have a pair ('Joe', 78) that we want to store in the hash map,
 * the key is 'Joe' so it determines the bucket in the hash map,
 * his index would be:  size_t ind = hash_func('Joe') & (capacity - 1);
 */
typedef size_t (*hash_func) (const_keyT);


/**
 * @typedef keyT_func
 * A function that receives a const_keyT, and returns 1 if it fulfills some condition, and 0 else
 */
typedef int (*keyT_func) (const_keyT);

/**
 * @typedef valueT_func
 * A function that changes the value of a valueT, in-place
 */
typedef void (*valueT_func) (valueT);

/**
 * @struct hashmap
 * @param buckets dynamic array of vectors which stores the values.
 * @param size the number of elements (pairs) stored in the hash map.
 * @param capacity the number of buckets in the hash map.
 * @param hash_func a function which "hashes" keys.
 */
typedef struct hashmap {
    vector **buckets;
    size_t size;
    size_t capacity; // num of buckets
    hash_func hash_func;
} hashmap;

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func);

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map);

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair);

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise (the value itself,
 * not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key);

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise. (if key not in map,
 * considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key);

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map);

/**
 * This function receives a hashmap and 2 functions, the first checks a condition on the keys,
 * and the seconds apply some modification on the values. The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a capital letter (A-Z),
 * and val_t_func multiples the number by 2, hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func, valueT_func valT_func);//const
#endif //HASHMAP_H_
