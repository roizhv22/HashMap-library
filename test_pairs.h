//
// Created by roizh on 18/05/2021.
//

#ifndef _TEST_PAIRS_H_
#define _TEST_PAIRS_H_

/**
 * Pairs like { char: int }
 * The key type is char *.
 * The value type is int *.
 */

#include <stdlib.h>
#include "pair.h"

void *float_key_cpy (const_keyT key)
{
  float *new_float = malloc (sizeof (float ));
  *new_float = *((float *) key);
  return new_float;
}

void *float_value_cpy (const_valueT val)
{
  float *new_float = malloc (sizeof (float ));
  *new_float = *((float *) val);
  return new_float;
}

int float_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(float *) key_1 == *(float *) key_2;
}

int float_value_cmp (const_valueT value_1, const_valueT value_2)
{
  return *(float *) value_1 == *(float *) value_2;
}

/**
 * Copies the char key of the pair.
 */
void *char_key_cpy (const_keyT key)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) key);
  return new_char;
}

void *char_value_cpy (const_valueT value)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) value);
  return new_char;
}

void *int_key_cpy (const_keyT key)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) key);
  return new_int;
}

/**
 * Copies the int key of the pair.
 */
void *int_value_cpy (const_valueT value)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) value);
  return new_int;
}

/**
 * Compares the char key of the pair.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(char *) key_1 == *(char *) key_2;
}

int char_value_cmp (const_valueT value_1, const_valueT value_2)
{
  return *(char *) value_1 == *(char *) value_2;
}

int int_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(int *) key_1 == *(int *) key_2;
}

/**
 * Compares the int key of the pair.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *(int *) val_1 == *(int *) val_2;
}


/**
 * Frees the char key of the pair.
 */
void basic_data_key_free (keyT* key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

void basic_data_value_free (valueT * value)
{
  if (value && *value)
    {
      free (*value);
      *value = NULL;
    }
}


/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a digit, else - 0
 */
int is_digit (const_keyT elem)
{
  char c = *((char *) elem);
  return (c > 47 && c < 58);
}

/*
 * checks if a int key is even. returns 1 if even 0 otherwise.
 */
int is_even(const_keyT key){
  int given_key = *((int*)key);
  if (given_key%2==0){
      return 1;
  }
  return 0;
}

/**
 * doubles the key pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem)
{
  *((int *) elem) *= 2;
}
/*
 * divided in half a given float value. To perform on float values.
 */
void dev_float_value(valueT value){
  *((float*)value) /= 2;
}

#endif //_TEST_PAIRS_H_
