#include "../../../../include/httpclient.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
  Create a new HashTable and return a pointer to it.
  Don't forget to free it's memory calling deletehash on it
  after usage.
  @return Return a pointer to the newly created HashTable.
  return (void *)-1 if memory allocation failed.
*/
HashTable *createhash(void) {
  HashTable *hash = malloc(sizeof(HashTable));
  if (hash == NULL) {
    return (void *)-1;
  }
  for (int i = 0; i < ARIA_DATA_STRUCTURES_HASH_MAX_SIZE; i++) {
    hash->bucket[i] = NULL;
  }
  return hash;
}

/*
  Safely deletes the HashTable, freeing the memory of all of it's elements.
  @param hash A pointer to a HashTable.
  @return Return 0 if successful. return (void *)-1 if hash is NULL.
*/
void *deletehash(HashTable *hash) {
  if (hash == NULL) {
    return (void *)-1;
  }
  LinkedList *list;
  KeyValue *kv;
  for (int i = 0; i < ARIA_DATA_STRUCTURES_HASH_MAX_SIZE; i++) {
    list = hash->bucket[i];
    if (hash->bucket[i] != NULL) {
      for (int j = 0; j < list->size; j++) {
        kv = getfromindex(list, j);
        free(kv->key);
        free(kv->value);
        free(kv);
      }
      deletelist(list);
    }
  }
  free(hash);
  return 0;
}

unsigned int hashfunc(char *str) {
  unsigned int hashValue = 0;
  for (; *str != '\0'; str++) {
    hashValue = 31 * hashValue + *str;
  }
  return hashValue % ARIA_DATA_STRUCTURES_HASH_MAX_SIZE;
}

/*
  Add a new key-value pair to the HashTable.
  @param hash A pointer to the HashTable.
  @param key A char pointer to the key.
  @param value A char pointer to the value.
  @return Return 0 if successful. return (void *)-1 if hash is NULL or
  in case of failure of memory allocation.
*/
void *addtohash(HashTable *hash, char *key, char *value) {
  if (hash == NULL) {
    return (void *)-1;
  }
  unsigned int index = hashfunc(key);
  LinkedList *temp = hash->bucket[index];
  KeyValue *kv = malloc(sizeof(KeyValue));
  if (kv == NULL) {
    return (void *)-1;
  }
  kv->key = strdup(key);
  kv->value = strdup(value);
  hash->bucket[index] = temp == NULL ? createlist() : temp;
  addtolist(hash->bucket[index], kv);
  return 0;
}

/*
  Gets a value from a hash using the provided key.
  @param hash A pointer to the HashTable.
  @param key A char pointer representing the key.
  @return Return a char pointer to the value. return "\0" if hash is NULL
  or if the provided key does not have a value.
*/
char *getfromhash(HashTable *hash, char *key) {
  if (hash == NULL) {
    return "\0";
  }
  unsigned int index = hashfunc(key);
  LinkedList *item = hash->bucket[index];
  if (item == NULL) {
    return "\0";
  }
  void *value;
  for (int i = 0; i < item->size; i++) {
    value = getfromindex(item, i);
    if (strcmp(((KeyValue *)value)->key, key) == 0) {
      return ((KeyValue *)value)->value;
    }
  }
  return "\0";
}
