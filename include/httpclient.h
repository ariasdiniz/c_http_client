#ifndef _ARIA_HTTP_CLIENT_H
#define _ARIA_HTTP_CLIENT_H

#define __BUFFER_SIZE 65536
#define ARIA_DATA_STRUCTURES_HASH_MAX_SIZE 100

// Node for the linked List
struct Node {
  void *value;
  struct Node *next;
};

// Linked List data structure
typedef struct LinkedList {
  struct Node *head;
  struct Node *tail;
  int size;
} LinkedList;

/*
  Create a new LinkedList.

  @return struct LinkedList* A pointer to the newly created LinkedList.
  Return NULL on failure to allocate memory.
  Don't forget to free memory after usage with deletelist(list).
*/
struct LinkedList *createlist(void);

/*
  Free the memory allocated by createlist().

  @param list A pointer to a LinkedList.
  @return void* Returns NULL when successful. -1 when the list param is NULL.
*/
void *deletelist(struct LinkedList *list);

/*
  Add a new element at the bottom of the list.

  @param list A pointer to a LinkedList.
  @param value A void pointer to the value to be added to the list.
  @return Returns 0 if successful. Returns -1 when an error occur.
*/
void *addtolist(struct LinkedList *list, void *value);

/*
  Gets the value of an specific index in the provided list.

  @param list A pointer to a LinkedList.
  @param index An integer representing the index.
  @returns Returns a pointer to the value of the index. return (void *)-1 in case of error.
*/
void *getfromindex(struct LinkedList *list, int index);

/*
  Remove an index from the provided list.

  @param list A pointer to a LinkedList.
  @param index An integer representing the index to be removed.
  @return Return 0 when successful. return (void *)-1 in case of error.
*/
void *removefromindex(struct LinkedList *list, int index);

// Hash table data structure
typedef struct HashTable {
  LinkedList *bucket[ARIA_DATA_STRUCTURES_HASH_MAX_SIZE];
} HashTable;

// KeyValue for the Hash Table
typedef struct KeyValue {
  char *key;
  char *value;
} KeyValue;

/*
  Create a new HashTable and return a pointer to it.
  Don't forget to free it's memory calling deletehash on it
  after usage.
  @return Return a pointer to the newly created HashTable.
  return (void *)-1 if memory allocation failed.
*/
HashTable *createhash(void);

/*
  Safely deletes the HashTable, freeing the memory of all of it's elements.
  @param hash A pointer to a HashTable.
  @return Return 0 if successful. return (void *)-1 if hash is NULL.
*/
void *deletehash(HashTable *hash);

/*
  Gets a value from a hash using the provided key.
  @param hash A pointer to the HashTable.
  @param key A char pointer representing the key.
  @return Return a char pointer to the value. return "\0" if hash is NULL
  or if the provided key does not have a value.
*/
char *getfromhash(HashTable *hash, char *key);

/*
  Add a new key-value pair to the HashTable.
  @param hash A pointer to the HashTable.
  @param key A char pointer to the key.
  @param value A char pointer to the value.
  @return Return 0 if successful. return (void *)-1 if hash is NULL or
  in case of failure of memory allocation.
*/
void *addtohash(HashTable *hash, char *key, char *value);

typedef struct HTTPRequest {
  char *method;
  char *path;
  HashTable *headers;
  char *body;
} HTTPRequest;

typedef struct HTTPResponse {
  char *body;
  LinkedList *headers;
  int status;
} HTTPResponse;


/**
 * @brief Parses the entire HTTP response and returns an HTTPResponse structure.
 *
 * @param response The raw HTTP response string to be parsed.
 * @return An HTTPResponse structure containing parsed information, or NULL on error.
 */
HTTPResponse *parse_response(char *response);

/**
 * @brief Frees the memory allocated for the HTTPResponse structure.
 *
 * @param response The HTTPResponse structure to be destroyed.
 * @return 0 on success, -1 if the input is NULL.
 */
int destroy_response(HTTPResponse *response);

/**
 * @brief Parses an HTTPRequest structure and returns an HTTP request string.
 *
 * @param request The HTTPRequest structure to be parsed.
 * @return A dynamically allocated string representing the entire HTTP request, or NULL on error.
 */
char *parse_request(HTTPRequest *request);

/**
 * @brief Sends an HTTP request to a specified server and port using a TCP socket.
 *
 * @param request The HTTPRequest structure containing the request information.
 * @param host The server's IP address or hostname.
 * @param port The port number on which the server is listening.
 * @param use_ssl Boolean indicating whether SSL/TLS should be used.
 * @return An HTTPResponse structure containing the parsed response, or NULL on error.
 */
HTTPResponse *shttp(HTTPRequest *request, char *host, unsigned int port, int use_ssl);

#endif // _ARIA_HTTP_CLIENT_H
