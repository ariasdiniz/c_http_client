#include "parse_request.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define _ARIA_C_HTTP_CLIENT_HTTPCLIENT_MAX_REQUEST_SIZE 65536

/**
 * @brief Iterates over a HashTable and appends formatted headers to a given string.
 *
 * @param hash The HashTable containing headers to be processed.
 * @param headers The string to which headers are appended.
 * @param buffer A temporary buffer for constructing header strings.
 * @return The updated string containing headers.
 */
static char *iterate_over_hash(HashTable *hash, char *headers, char *buffer) {
  LinkedList *list;
  KeyValue *kv;

  // Handle the case where the hash table is NULL
  if (hash == NULL) {
    strcat(headers, "\n");
    return headers;
  }

  // Iterate over hash table buckets
  for (int i = 0; i < ARIA_DATA_STRUCTURES_HASH_MAX_SIZE; i++) {
    if (hash->bucket[i] == NULL || hash->bucket[i]->size == 0) {
      continue;
    }

    list = hash->bucket[i];

    // Iterate over the linked list in each bucket
    for (int j = 0; j < hash->bucket[i]->size; j++) {
      kv = ((KeyValue *)getfromindex(list, j));
      // Format and append the header to the string
      snprintf(
        buffer, 
        (sizeof(char) * _ARIA_C_HTTP_CLIENT_HTTPCLIENT_MAX_REQUEST_SIZE), 
        "%s: %s\n",
        kv->key, 
        kv->value
      );
      strcat(headers, buffer);
    }
  }

  // Append an empty line to indicate the end of headers
  strcat(headers, "\n");
  return headers;
}

/**
 * @brief Mounts an HTTP request string using the provided HTTPRequest structure.
 *
 * @param request The HTTPRequest structure containing request information.
 * @return A dynamically allocated string representing the entire HTTP request, or NULL on error.
 */
static char *mount_http(HTTPRequest *request) {
  char *request_text = malloc(_ARIA_C_HTTP_CLIENT_HTTPCLIENT_MAX_REQUEST_SIZE);
  memset(request_text, '\0', _ARIA_C_HTTP_CLIENT_HTTPCLIENT_MAX_REQUEST_SIZE);

  // Check if memory allocation is successful
  if (request_text == NULL) {
    return NULL;
  }

  char buffer[_ARIA_C_HTTP_CLIENT_HTTPCLIENT_MAX_REQUEST_SIZE];

  // Construct the request line with method, path, and HTTP version
  snprintf(buffer, sizeof(buffer), "%s %s HTTP/1.1\n", request->method, request->path);
  strcat(request_text, buffer);

  // Add headers to the request text
  iterate_over_hash(request->headers, request_text, buffer);

  // Append the request body
  strcat(request_text, request->body);
  strcat(request_text, "\n");

  return request_text;
}

/**
 * @brief Parses an HTTPRequest structure and returns an HTTP request string.
 *
 * @param request The HTTPRequest structure to be parsed.
 * @return A dynamically allocated string representing the entire HTTP request, or NULL on error.
 */
char *parse_request(HTTPRequest *request) {
  // Check if the input request is NULL
  if (request == NULL) {
    return NULL;
  }

  // Mount the HTTP request string
  char *response = mount_http(request);

  // Delete the HashTable containing headers and free the HTTPRequest structure
  deletehash(request->headers);
  free(request);

  return response;
}