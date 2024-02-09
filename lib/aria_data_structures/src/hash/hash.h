#ifndef ARIA_DATA_STRUCTURES_HASH_H
#define ARIA_DATA_STRUCTURES_HASH_H

#define ARIA_DATA_STRUCTURES_HASH_MAX_SIZE 100
#include "../linkedlist/list.h"

typedef struct HashTable {
  LinkedList *bucket[ARIA_DATA_STRUCTURES_HASH_MAX_SIZE];
} HashTable;

typedef struct KeyValue {
  char *key;
  char *value;
} KeyValue;

HashTable *createhash(void);
void *deletehash(HashTable *hash);
char *getfromhash(HashTable *hash, char *key);
void *addtohash(HashTable *hash, char *key, char *value);

#endif // ARIA_DATA_STRUCTURES_HASH_H
