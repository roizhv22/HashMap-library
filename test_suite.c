#include "test_suite.h"
#include "hashmap.h"
#include "test_pairs.h"
#include "hash_funcs.h"
#include "pair.h"

#define NUM_OF_PAIRS 250 //careful from char overflow as some
//functions checks the char pairs and we can only have 256 keys.
#define INT_KEY_BASE_VALUE 100
#define FLOAT_VALUE_BASE_VAL 50.5
#define DELTA_FOR_FLOAT_VAL 6.6
#define INT_VALUE_BASE 500
#define INT_VALUE_DELTA 30
#define CHAR_KEY_BASE 6
#define NUM_OF_DIGITS 10

/*
 * creates MUM_OF_PAIRS int-float general pairs. FREE NEEDED!
 * returns NULL if malloc fails.
 */
pair* create_int_float_pairs(){
  pair* pairs = malloc (sizeof (pair)*NUM_OF_PAIRS);
  if (pairs == NULL){
      return NULL;
  }
  int base_key = INT_KEY_BASE_VALUE;
  float base_val = FLOAT_VALUE_BASE_VAL;
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      pair *pr = pair_alloc (&base_key, &base_val, int_key_cpy,float_value_cpy,
                             int_key_cmp, float_value_cmp,
                             basic_data_key_free, basic_data_value_free);
      pairs[i] = *pr;
      free (pr);
      pr = NULL;
      base_key++;
      base_val += DELTA_FOR_FLOAT_VAL;
    }
  return pairs;
}
/*
 * creates NUM_OF_PAIRS char-int pairs. returns the array - FREE NEEDED!
 * NULL ptr will be returned if malloc fails.
 */
pair* create_char_int_pairs(){
  pair* pairs = malloc (sizeof (pair)*NUM_OF_PAIRS);
  if (pairs == NULL){
      return NULL;
  }
  int base_val = INT_VALUE_BASE;
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      char base_key = (char) (i+CHAR_KEY_BASE);
      pair *pr = pair_alloc (&base_key, &base_val, char_key_cpy,int_value_cpy,
                              char_key_cmp, int_value_cmp,
                              basic_data_key_free, basic_data_value_free);
      pairs[i] = *pr;
      free (pr);
      pr = NULL;
      base_val += INT_VALUE_DELTA;
    }
  return pairs;
}
/*
 * frees the pairs! number of pairs to be free is given in the marco NUM
 * OF PAIRS
 */
void free_pair_list(pair** pairs){
  free (*pairs);
}
/*@param: receives an empty hash_map that is initialize to the pairs
 * type and a pairs list.
 * general insertion test. performing assert checks on different scenarios.
 * due note that this function frees the hash_map and pairs!
 */
void general_insert_test (hashmap* hash_map,pair* pairs)
{
  size_t old_capacity = hash_map->capacity;
  for (size_t  i = 0; i < NUM_OF_PAIRS; ++i)
    {
      assert(hashmap_insert (hash_map, &(pairs[i])));
      assert(hash_map->size == i+1); // checks size increase correctly.
      //standard insertion test.
      double prev_load = (double) (hash_map->size-1)/hash_map->capacity;
      if (prev_load>HASH_MAP_MAX_LOAD_FACTOR){
          assert(hash_map->capacity == old_capacity*HASH_MAP_GROWTH_FACTOR);
          old_capacity = hash_map->capacity;
          //checks that the capacity increase is performing as exepected.
      }
    }
  size_t hash_size = hash_map->size;
  size_t hash_capacity = hash_map->capacity;
  for (int i = 0; i < NUM_OF_PAIRS/4; ++i)
    {
      assert(hashmap_insert (hash_map, NULL) == 0);
      assert(hash_map->size == hash_size);
      assert(hash_map->capacity == hash_capacity);
      // trying to enter NULL ptr.
    }
  for (int i = 0; i < 50; ++i)
    {
      assert(hashmap_insert (hash_map, &(pairs[50+i]))==0);
      assert(hash_map->size == hash_size);
      assert(hash_map->capacity == hash_capacity);
      // trying to insert keys that are already in the hashmap. testing the
      // at function at the same time.
      // make sure no changes to the size/capacity are made. 
    }
  hashmap_free (&hash_map);
  free_pair_list (&pairs);
}

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert(void){
  pair* char_int_pairs = create_char_int_pairs();
  if (char_int_pairs == NULL){
      exit (1); // malloc fails.
    }
  hashmap * char_int_hash = hashmap_alloc (hash_char);
  if (char_int_hash == NULL){
      free_pair_list (&char_int_pairs);
      exit(1); // malloc fails.
    }
  general_insert_test (char_int_hash, char_int_pairs);
  
  pair* int_float_pairs = create_int_float_pairs();
  if (int_float_pairs == NULL){
      exit (1); // malloc fails.
    }
  hashmap * int_float_hash = hashmap_alloc (hash_int);
  if (int_float_hash == NULL){
      free_pair_list (&int_float_pairs);
      exit(1); // malloc fails.
    }
  general_insert_test (int_float_hash, int_float_pairs);//the test functions
  //free the hash_map and pairs!
}
/*
 * @param: receives an empty hash_map that is initialize to the pairs
 * type and a pairs list.
 * general testing on the at, checking invalid input and esixting
 * of a given key in the table.
 */
void general_at_test(hashmap* hash_map, pair* pairs){
  for (int i = 0; i < NUM_OF_PAIRS/2; ++i)
    {
      hashmap_insert (hash_map, &(pairs[i])); //insert half of the pairs.
    }
  for (int i = 0; i < (int) NUM_OF_PAIRS/4; ++i)
    {
      assert(hashmap_at (hash_map,NULL) == NULL);
      assert((hashmap_at (NULL, pairs[i].key)) == NULL);
      assert((hashmap_at (NULL, NULL)) == NULL);
      //checks invalid input.
    }
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      if (i<NUM_OF_PAIRS/2){
          assert((hashmap_at (hash_map, pairs[i].key)) != NULL);
          //checks on pair that in the table.
      }
      else{
          assert((hashmap_at (hash_map, pairs[i].key)) == NULL);
          //checks on a pair that isn't in the table.
      }
    }
  hashmap_free(&hash_map);
  free_pair_list(&pairs);
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at(void)
{
  pair *char_int_pairs = create_char_int_pairs ();
  if (char_int_pairs == NULL)
    {
      exit (1); // malloc fails.
    }
  hashmap *char_int_hash = hashmap_alloc (hash_char);
  if (char_int_hash == NULL)
    {
      free_pair_list (&char_int_pairs);
      exit (1); // malloc fails.
    }
  general_at_test (char_int_hash, char_int_pairs);//frees resources.
  pair *int_float_pairs = create_int_float_pairs();
  if (int_float_pairs == NULL)
    {
      exit (1); // malloc fails.
    }
  hashmap *int_float_hash = hashmap_alloc (hash_int);
  if (int_float_hash == NULL)
    {
      free_pair_list (&int_float_pairs);
      exit (1); // malloc fails.
    }
  general_at_test (int_float_hash, int_float_pairs); //frees resources
}
/***
 *
 * @param hash_map an empty hash_map that is initialize to the given pairs
 * type
 * @param pairs pair list.
 * performing erases on different scenarios.
 */
void general_erase_test(hashmap* hash_map, pair* pairs){
  size_t initial_hash_size = hash_map->size;
  size_t initial_hash_capacity = hash_map->capacity;
  for (int i = 0; i <NUM_OF_PAIRS ; ++i)
    {
      assert((hashmap_erase (hash_map, pairs[i].key)) == 0);
      //trying to erase from an empty table.
      assert(hash_map->size == initial_hash_size);
      assert(hash_map->capacity == initial_hash_capacity);
      //checks no changes in size/capacity.
      assert((hashmap_erase (NULL, pairs[i].key))==0);
      assert((hashmap_erase (hash_map,NULL))==0);
      //invalid inputs.
    }
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      hashmap_insert (hash_map, &(pairs[i]));
    }
  initial_hash_size = NUM_OF_PAIRS;
  initial_hash_capacity = hash_map->capacity;
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      assert((hashmap_erase (hash_map, (pairs[i].key))) == 1);
      initial_hash_size--;
      assert(hash_map->size == initial_hash_size);
      double prev_load = (double)(hash_map->size+1)/hash_map->capacity;
      if (prev_load<HASH_MAP_MIN_LOAD_FACTOR){
          assert(initial_hash_capacity/2 == hash_map->capacity);
          initial_hash_capacity/=2;
      }
    }
    //checking size/ capacity through the delition process.
  hashmap_free(&hash_map);
  free_pair_list(&pairs);
}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase(void){
  pair *char_int_pairs = create_char_int_pairs ();
  if (char_int_pairs == NULL)
    {
      exit (1); // malloc fails.
    }
  hashmap *char_int_hash = hashmap_alloc (hash_char);
  if (char_int_hash == NULL)
    {
      free_pair_list (&char_int_pairs);
      exit (1); // malloc fails.
    }
  general_erase_test(char_int_hash, char_int_pairs);//frees resources.
  pair *int_float_pairs = create_int_float_pairs();
  if (int_float_pairs == NULL)
    {
      exit (1); // malloc fails.
    }
  hashmap *int_float_hash = hashmap_alloc (hash_int);
  if (int_float_hash == NULL)
    {
      free_pair_list (&int_float_pairs);
      exit (1); // malloc fails.
    }
  general_erase_test(int_float_hash, int_float_pairs); //frees resources
}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap
 * library. If hashmap_get_load_factor fails at some points,
 * the functions exits with
 * exit code 1.
 */
void test_hash_map_get_load_factor(void){
  hashmap * dummy_hash = hashmap_alloc (hash_char);
  dummy_hash->capacity = 0;
  assert(hashmap_get_load_factor (dummy_hash) == -1);//checking case of
  // division in 0.
  hashmap_free (&dummy_hash);
  pair * pairs = create_char_int_pairs();
  hashmap* hash_map_test = hashmap_alloc (hash_char);
  size_t size_test = 0;
  size_t capacity_test = HASH_MAP_INITIAL_CAP ;
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      double load = (double)size_test/capacity_test;
      assert(load == hashmap_get_load_factor (hash_map_test));
      if (load>HASH_MAP_MAX_LOAD_FACTOR){
        capacity_test*=  HASH_MAP_GROWTH_FACTOR;
      }
      hashmap_insert (hash_map_test, &(pairs[i]));
      size_test++;
      //checks load_factor while increasing the size of the table.
    }
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      double load = (double)size_test/capacity_test;
      assert(load == hashmap_get_load_factor (hash_map_test));
      if (load<HASH_MAP_MIN_LOAD_FACTOR){
          capacity_test/= HASH_MAP_GROWTH_FACTOR;
        }
      hashmap_erase(hash_map_test, pairs[i].key);
      size_test--;
      //checks load_factor while removing pairs from table.
    }
  hashmap_free (&hash_map_test);
  free_pair_list (&pairs);
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if(){
  pair* int_float_pairs = create_int_float_pairs();
  pair* char_int_pairs = create_char_int_pairs();
  hashmap * int_float_hash = hashmap_alloc (hash_int);
  hashmap * char_int_hash = hashmap_alloc (hash_char);
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      hashmap_insert (int_float_hash, &(int_float_pairs[i]));
      hashmap_insert (char_int_hash, &(char_int_pairs[i]));
    }
  int num_of_changes_float_int, num_of_changes_char_int;
  num_of_changes_float_int = hashmap_apply_if
      (int_float_hash, is_even, dev_float_value);
  num_of_changes_char_int = hashmap_apply_if
      (char_int_hash, is_digit, double_value);
  assert(num_of_changes_float_int == NUM_OF_PAIRS/2);
  //checks num of changes is correct.
  assert(num_of_changes_char_int == NUM_OF_DIGITS);
  for (int i = 0; i < NUM_OF_PAIRS; ++i)
    {
      valueT cur_pair_f_i_val = int_float_pairs[i].value;
      keyT cur_pair_f_i_key = int_float_pairs[i].key;
      valueT hash_val1 = hashmap_at (int_float_hash, cur_pair_f_i_key);
      if(*((int*)cur_pair_f_i_key) % 2 == 0){
        assert(*((float*)cur_pair_f_i_val) / 2 == *((float*)hash_val1));
      }
      else{
          assert(*((float*)cur_pair_f_i_val) == *((float*)hash_val1));
        }
      // checks that each change is correct
      valueT cur_pair_c_i_val = char_int_pairs[i].value;
      keyT cur_pair_c_i_key = char_int_pairs[i].key;
      valueT hash_val2 = hashmap_at (char_int_hash, cur_pair_c_i_key);
      char c = *((char *) cur_pair_c_i_key);
      if (c > 47 && c < 58){
          assert(*((int*)cur_pair_c_i_val)*2 == *((int*)hash_val2));
      }
      else{
          assert(*((int*)cur_pair_c_i_val) == *((int*)hash_val2));
        }
    }
  hashmap_free (&int_float_hash);
  hashmap_free (&char_int_hash);
  free_pair_list (&int_float_pairs);
  free_pair_list (&char_int_pairs);
}

