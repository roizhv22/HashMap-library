//
// Created by roizh on 17/05/2021.
//

#include "hashmap.h"
#include "vector.h"

#define INCREASE 99
#define DECREASE 95

size_t get_ind_from_hash(const hashmap* hash_map, const_keyT key){
   size_t val = hash_map->hash_func(key) & (hash_map->capacity -1);
  return val;
}

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func){
  hashmap* map = malloc (sizeof(hashmap));
  if (map == NULL){
      return NULL;
  }
  map->size = 0;
  map->capacity = HASH_MAP_INITIAL_CAP;
  map->hash_func = func;
  vector** buckets = malloc (sizeof (vector*)*map->capacity);
  if(buckets == NULL){
      free (map);
      return NULL;
  }
  map->buckets = buckets;
  for (size_t i = 0; i <map->capacity ; ++i)
    {
      vector* vec = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (vec == NULL){
          free (buckets);
          free(map);
          return NULL;
      }
      map->buckets[i] = vec;
    }
  return map;
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map){
  for (size_t  i = 0; i < (*p_hash_map)->capacity; ++i)
    {
      vector_free (&((*p_hash_map)->buckets[i])); //ptr->ptr
    }
  free ((*p_hash_map)->buckets);
  free (*p_hash_map);
  *p_hash_map = NULL;}

/**
* This function returns the load factor of the hash map.
* @param hash_map a hash map.
* @return the hash map's load factor, -1 if the function failed.
*/
double hashmap_get_load_factor (const hashmap *hash_map){
  if(hash_map->capacity <= 0){
      return -1;
  }
  double load = (double)hash_map->size/hash_map->capacity;
  return load;
}
/*
 * The rehash function rehashes the table to a new vector array due to size
 * changes. each time we add the given pair to the new array and freeing
 * the vector after all pairs has been pushed back to the new vector.
 * Returns 0 if failed otherwise 1.
 */
int rehash(hashmap* hash_map, vector*** new_bucket_lst, size_t new_capacity){
  for (size_t  i = 0; i < hash_map->capacity; ++i)
    {
      for (size_t  j = 0; j <hash_map->buckets[i]->size ; ++j)
        {
          pair* cur_pair = (pair*)(hash_map->buckets[i]->data[j]);
          size_t key = hash_map->hash_func(cur_pair->key) & (new_capacity-1);
          if (vector_push_back ((*new_bucket_lst)[key], cur_pair) != 1){
              return 0; // failure!
          }
        }
      vector_free (&(hash_map->buckets[i]));
    }
  return 1;
}
/*
 * Increase/decrease the has_table size using the rehash function.
 * The function creates a new buckets array, hashes the old hashmap pairs
 * to it using the hash function with the bitwise operation and then frees
 * the old array, switching the ptr in the hash_map struct to the relevant one.
 * Function returns 1 upon success 0 otherwise.
 */
int hashmap_increase_decrease(hashmap* hash_map, int flag){
  size_t new_capacity;
  if (flag == INCREASE){
    new_capacity = hash_map->capacity*HASH_MAP_GROWTH_FACTOR;
  }
  else{
    new_capacity = hash_map->capacity/HASH_MAP_GROWTH_FACTOR;
  }
  vector ** new_bucket = malloc (sizeof(vector*)*new_capacity);
  if (new_bucket == NULL){
      return 0;
  }
  for (size_t  i = 0; i <new_capacity ; ++i)
    {
      vector * vec = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (vec == NULL){
          for (size_t j = 0; j < i; ++j)
            {
              vector_free (&(new_bucket[j])); // release all previous vectors.
            }
          free (new_bucket); // release bucket_malloc.
          return 0;
      }
      new_bucket[i] = vec;
    }
  if (rehash (hash_map, &new_bucket, new_capacity) == 0){
      for (size_t  i = 0; i < new_capacity; ++i) //free all the vector in case
        // of failure.
        {
          vector_free (&(new_bucket[i]));
        }
      free (new_bucket);
      return 0;
  }
  vector ** temp_ptr = hash_map->buckets;
  hash_map->buckets = new_bucket;
  hash_map->capacity = new_capacity;
  free (temp_ptr);
  return 1;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise
 * (the value itself,
 * not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key){
  if ((key ==NULL)||(hash_map == NULL)){
      return NULL;
  }
  size_t ind_key = get_ind_from_hash (hash_map, key);
  vector* vec = hash_map->buckets[ind_key];
  for (size_t  i = 0; i < vec->size; ++i)
    {
      pair* cur_pair = (pair*)vec->data[i];
      if (cur_pair->key_cmp(cur_pair->key, key) == 1){
          return cur_pair->value;
        }
    }
  return NULL;
}

/**
* Inserts a new in_pair to the hash map.
* The function inserts *new*, *copied*, *dynamically allocated* in_pair,
* NOT the in_pair it receives as a parameter.
* @param hash_map the hash map to be inserted with new element.
* @param in_pair a in_pair the hash map would contain.
* @return returns 1 for successful insertion, 0 otherwise.
*/
int hashmap_insert (hashmap *hash_map, const pair *in_pair){
  if (in_pair == NULL){
      return 0;
  }
  if (hashmap_at (hash_map, in_pair->key) != NULL){
      return 0;
  }
  if (hashmap_get_load_factor (hash_map) >= HASH_MAP_MAX_LOAD_FACTOR)
    {
      if (hashmap_increase_decrease (hash_map, INCREASE) != 1)
        {
          return 0;
        }
    }
  size_t hash_ind = get_ind_from_hash (hash_map, in_pair->key);
  if(vector_push_back (hash_map->buckets[hash_ind], in_pair) != 1){
      return 0;
  }
  hash_map->size += 1;
  return 1;
}


/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
 * (if key not in map,
 * considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key){
  if ((hash_map == NULL)||key == NULL){
      return 0;//invalid input.
  }
  if (hash_map->size == 0){
      return 0; //no pairs to delete.
  }
  if (hashmap_at (hash_map, key) == NULL){
      return 0;
  }
  if (hashmap_get_load_factor (hash_map)<=HASH_MAP_MIN_LOAD_FACTOR){
      if (hashmap_increase_decrease (hash_map, DECREASE) != 1){
          return 0;
      }
  }
  size_t key_ind = get_ind_from_hash (hash_map, key);
  vector * vec = hash_map->buckets[key_ind];
  for (size_t  i = 0; i < vec->size; ++i)
    {
      pair* cur_pair = (pair*)vec->data[i];
      if (cur_pair->key_cmp(cur_pair->key, key) == 1){
          if(vector_erase (vec, i) == 0){
              return 0;
          }
          else{
              hash_map->size -= 1;
              break;
          }}}
  return 1;
}

/**
 * This function receives a hashmap and 2 functions, the first checks a
 * condition on the keys,and the seconds apply some modification on the values.
 * The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a
 * capital letter (A-Z),and val_t_func multiples the number by 2,
 * hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the return
 * value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return 1
 * if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                      valueT_func valT_func){
  int changed_vals = 0;
  for (size_t  i = 0; i <hash_map->capacity ; ++i)
    {
      for (size_t  j = 0; j <hash_map->buckets[i]->size ; ++j)
        {
          pair* cur_pair = (pair*)(hash_map->buckets[i]->data[j]);
          if (keyT_func(cur_pair->key) == 1){
            valT_func(cur_pair->value);
            changed_vals++;
          }
        }
    }
  return changed_vals;
}


