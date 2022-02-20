#include "pair.h"

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
    const pair_key_cpy key_cpy, const pair_value_cpy value_cpy,
    const pair_key_cmp key_cmp, const pair_value_cmp value_cmp,
    const pair_key_free key_free, const pair_value_free value_free)
{
  pair *p = malloc (sizeof (pair));
  p->key = key_cpy (key);
  p->value = value_cpy (value);
  p->key_cpy = key_cpy;
  p->value_cpy = value_cpy;
  p->key_cmp = key_cmp;
  p->value_cmp = value_cmp;
  p->key_free = key_free;
  p->value_free = value_free;
  return p;
}

/**
 * Creates a new (dynamically allocated) copy of the given old_pair.
 * @param old_pair old_pair to be copied.
 * @return new dynamically allocated old_pair if succeeded, NULL otherwise.
 */
void *pair_copy (const void *p)
{
  if (!p)
    {
      return NULL;
    }
  const pair *old_pair = (const pair *) p;
  pair *new_pair = pair_alloc (old_pair->key, old_pair->value,
                               old_pair->key_cpy, old_pair->value_cpy,
                               old_pair->key_cmp, old_pair->value_cmp,
                               old_pair->key_free, old_pair->value_free);
  return new_pair;
}


int pair_cmp (const void *p1, const void *p2)
{
  if (!p1 || !p2)
    {
      return 0;
    }
  const pair *pair1 = (const pair *) p1;
  const pair *pair2 = (const pair *) p2;

  int key_cmp = pair1->key_cmp (pair1->key, pair2->key);
  int val_cmp = pair1->value_cmp (pair1->value, pair2->value);
  return key_cmp && val_cmp;
}

/**
 * This function frees a pair and everything it allocated dynamically.
 * @param p_pair pointer to dynamically allocated pair to be freed.
 */
void pair_free (void ** p)
{
  if (!p || !(*p))
    {
      return;
    }
  pair **p_pair = (pair **) p;
  (*p_pair)->key_free (&((*p_pair)->key));
  (*p_pair)->value_free (&(*p_pair)->value);
  free (*p_pair);
  *p_pair = NULL;
}
