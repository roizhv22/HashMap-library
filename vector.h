#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>

/**
 * @def VECTOR_INITIAL_CAP
 * The initial capacity of the vector.
 */
#define VECTOR_INITIAL_CAP 16UL

/**
 * @def VECTOR_GROWTH_FACTOR
 * The growth factor of the vector.
 */
#define VECTOR_GROWTH_FACTOR 2UL

/**
 * @def VECTOR_MAX_LOAD_FACTOR
 * The maximal load factor the vector can be in before
 * extension (vector need to be extended if the load factor is >0.75).
 */
#define VECTOR_MAX_LOAD_FACTOR 0.75

/**
 * @def VECTOR_MIN_LOAD_FACTOR
 * The minimal load factor the vector can be in before
 * size decreasing (vector need to be decreased if the load factor is <0.25).
 */
#define VECTOR_MIN_LOAD_FACTOR 0.25

/**
 * @typedef vector_elem_cpy
 * Function which receive an element of the type stored in the vector
 * and returns a dynamically allocated copy of it.
 */
typedef void *(*vector_elem_cpy)(const void *);

/**
 * @typedef vector_elem_cmp
 * Function which receives two elements of the type stored in the vector
 * and returns 1 if they are equal, 0 otherwise.
 */
typedef int (*vector_elem_cmp)(const void *, const void *);

/**
 * @typedef vector_elem_free
 * Function which receives a pointer to an element of the type stored in the vector
 * vector and frees it.
 */
typedef void (*vector_elem_free)(void **);

/**
 * @struct vector - a generic vector struct.
 * @param capacity - the capacity of the vector.
 * @param size - the current size of the vector.
 * @param data - the values stored inside the vector.
 * @param elem_copy_func - a function which copies (returns
 * a dynamically allocates copy) of elements of the type stored in the vector.
 * @param elem_cmp_func - a function which compares the elements
 * stored in the vector.
 * @param elem_free_func - a function which frees the elements stored
 * in the vector.
 */
typedef struct vector {
  size_t capacity;
  size_t size;
  void** data;
  vector_elem_cpy elem_copy_func;
  vector_elem_cmp elem_cmp_func;
  vector_elem_free elem_free_func;
} vector;

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc(vector_elem_cpy elem_copy_func, vector_elem_cmp elem_cmp_func,
                     vector_elem_free elem_free_func);

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free(vector **p_vector);

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists (the element itself, not a copy of it),
 * NULL otherwise.
 */
void *vector_at(const vector *vector, size_t ind);

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find(const vector *vector, const void *value);

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back(vector *vector, const void *value);

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor(const vector *vector);

/**
 * Removes the element at the given index from the vector. alters the indices of the remaining
 * elements so that there are no empty indices in the range [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase(vector *vector, size_t ind);

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear(vector *vector);

#endif //VECTOR_H_
