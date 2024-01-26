# Aria's Data Structures

## Overview
Aria's Data Structures is a C library providing efficient implementations of common data structures, accessible via a single header file `datastructures.h`. This library is ideal for educational purposes and for use in C projects that require basic data structure implementations.

## Features
- **Hash Table**: Efficient key-value storage.
- **Linked List**: Dynamic linear data storage.
- **Queue**: FIFO (First-In-First-Out) data structure.
- **Stack**: LIFO (Last-In-First-Out) data structure.

## Getting Started
### Prerequisites
- A C compiler (e.g., GCC, Clang).
- Basic understanding of C programming.

### Installation
1. Clone or download this repository.
2. Include `datastructures.h` in your C project.

### Usage Examples
#### Hash Table
```c
#include "datastructures.h"

// Creating a hash table
HashTable *myHash = createhash();
// Adding key-value pairs
addtohash(myHash, "key1", "value1");
addtohash(myHash, "key2", "value2");
// Retrieving a value
char *value = getfromhash(myHash, "key1");
// Deleting the hash table
deletehash(myHash);
```

#### Linked List
```c
#include "datastructures.h"

// Creating a linked list
struct LinkedList *myList = createlist();
// Adding elements
addtolist(myList, "data1");
addtolist(myList, "data2");
// Getting an element
char *data = getfromindex(myList, 1);
// Deleting the linked list
deletelist(myList);
```

#### Queue
```c
#include "datastructures.h"

// Creating a queue
struct Queue *myQueue = createqueue();
// Adding elements
addtoqueue(myQueue, "item1");
addtoqueue(myQueue, "item2");
// Removing an element
char *item = popfromqueue(myQueue);
// Deleting the queue
deletequeue(myQueue);
```

#### Stack
```c
#include "datastructures.h"

// Creating a stack
struct Stack *myStack = createstack();
// Adding elements
addtostack(myStack, "element1");
addtostack(myStack, "element2");
// Removing an element
char *element = popfromstack(myStack);
// Deleting the stack
deletestack(myStack);
```

## API Reference
### Hash Table
- `HashTable *createhash()`: Creates a new HashTable.
- `void *deletehash(HashTable *hash)`: Deletes a HashTable.
- `void *addtohash(HashTable *hash, char *key, char *value)`: Adds a key-value pair to HashTable.
- `char *getfromhash(HashTable *hash, char *key)`: Retrieves value for a given key from HashTable.

### Linked List
- `struct LinkedList *createlist()`: Creates a new LinkedList.
- `void *deletelist(struct LinkedList *list)`: Deletes a LinkedList.
- `void *addtolist(struct LinkedList *list, void *value)`: Adds a new element to LinkedList.
- `void *getfromindex(struct LinkedList *list, int index)`: Gets a value from LinkedList by index.

### Queue
- `struct Queue *createqueue()`: Creates a new Queue.
- `void *deletequeue(struct Queue *queue)`: Deletes a Queue.
- `void *addtoqueue(struct Queue *queue, void *value)`: Adds a new element to Queue.
- `void *popfromqueue(struct Queue *queue)`: Removes and returns the first element of Queue.

### Stack
- `struct Stack *createstack()`: Creates a new Stack.
- `void *deletestack(struct Stack *stack)`: Deletes a Stack.
- `void *addtostack(struct Stack *stack, void *value)`: Adds a new element to Stack.
- `void *popfromstack(struct Stack *stack)`: Removes and returns the top element of Stack.
