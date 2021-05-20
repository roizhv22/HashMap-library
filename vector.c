//
// Created by roizh on 17/05/2021.
//

#include "vector.h"

#define SUCSSES 1
#define FAIL 0

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector
 * (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the
 * vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc(vector_elem_cpy elem_copy_func,
                     vector_elem_cmp elem_cmp_func,
                     vector_elem_free elem_free_func){
  vector* vec = malloc (sizeof (vector));
  if(vec == NULL){
    return NULL;
  }
  vec->size = 0;
  vec->capacity = VECTOR_INITIAL_CAP;
  void** data = malloc (sizeof (void*) * VECTOR_INITIAL_CAP);
  if(data == NULL){
      free (vec);
      return NULL;
  }
  vec->data = data;
  vec->elem_cmp_func = elem_cmp_func; vec->elem_copy_func = elem_copy_func;
  vec->elem_free_func =elem_free_func;
  return vec;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free(vector **p_vector){
  vector *vec;
  vec = *p_vector;
  for (size_t i = 0; i < vec->size; ++i)
    {
      vec->elem_free_func(&(vec->data[i]));
    }
  free ((*p_vector)->data);
  free (*p_vector);
  *p_vector = NULL;
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists
 * (the element itself, not a copy of it),
 * NULL otherwise.
 */
void *vector_at(const vector *vector, size_t ind){
  if((ind >= vector->size)||(vector == NULL)){
    return NULL;
  }
  return vector->data[ind]; //returns a pointer to the data that's inside
  // the vector not a copy of the data. this way it can be altered.
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector
 * ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find(const vector *vector, const void *value){
  if((vector == NULL) ||(value == NULL)){
      return -1;
  }
  for (size_t i = 0; i<vector->size ; ++i)
    {
      if(vector->elem_cmp_func(vector->data[i], value) == 1){
        return i; // element found so return the index.
      }
    }
  return -1; //not found.
}
/*
 * Increasing the vector capacity according to the instructions.
 */
int vector_increase_cap(vector* vec){
  void** temp = realloc (vec->data, sizeof(void*) *
  (vec->capacity * VECTOR_GROWTH_FACTOR));
  if(temp == NULL){
      return FAIL;
  }
  vec->data = temp;
  vec->capacity *=2;
  return SUCSSES;
}
/*
 * Decreasing the vector capacity according to the instructions.
 */
int vector_decrease_cap(vector* vec){
  void** temp = realloc (vec->data, sizeof (void*)*
  (vec->capacity/VECTOR_GROWTH_FACTOR));
  if(temp == NULL){
    return FAIL;
  }
  vec->data = temp;
  vec->capacity /= 2;
  return SUCSSES;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor(const vector *vector){
  if(vector->capacity <=0){
      return -1;
  }
  double load_factor = (double)vector->size/vector->capacity;
  return load_factor;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back(vector *vector, const void *value){
  if(vector == NULL){
    return FAIL;
  }
  void* new_data = vector->elem_copy_func(value); //TODO checks fails?
  vector->data[vector->size] = new_data;
  vector->size += 1;
  if(vector_get_load_factor (vector)>VECTOR_MAX_LOAD_FACTOR){
      if(vector_increase_cap (vector) == 0){
          return FAIL; //case of allocation failure.
        }
    }
  return SUCSSES;
}


/**
 * Removes the element at the given index from the vector. alters the indices
 * of the remaining elements so that there are no empty indices in the range
 * [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase(vector *vector, size_t ind){
  if(vector == NULL){
      return FAIL;
  }
  if(vector_get_load_factor (vector) < VECTOR_MIN_LOAD_FACTOR){
      if(vector_decrease_cap (vector) == 0){
          return FAIL; // realloc fails.
        }
    }
  vector->elem_free_func(&(vector->data[ind]));// frees the element.
  for (size_t i = ind +1; i <vector->size ; ++i) // fixing the data array.
    {
      vector->data[i-1] = vector->data[i];
    }
  vector->size -= 1;
  return SUCSSES;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear(vector *vector){
  while(vector->size != 0)
    {
      vector_erase (vector, 0);
    }
}



