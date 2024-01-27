#include "httpclient.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define _ARIA_C_HTTP_CLIENT_HTTPCLIENT_MAX_REQUEST_SIZE 999999

static char *iterate_over_hash(HashTable *hash, char *headers, char *buffer) {
  LinkedList *list;
  KeyValue *kv;
  for (int i = 0; i < ARIA_DATA_STRUCTURES_HASH_MAX_SIZE; i++) {
    if (hash->bucket[i] == NULL) {
      continue;
    }
    list = hash->bucket[i];
    for (int j = 0; j < hash->bucket[i]->size; j++) {
      kv = ((KeyValue *)getfromindex(list, j));
      snprintf(buffer, sizeof(buffer), "%s: %s\n", kv->key, kv->value);
      strcat(headers, buffer);
    }
  }
  strcat(headers, "\n");
  return headers;
}

static char *mount_http(HTTPRequest *request) {
  char *request_text = malloc(_ARIA_C_HTTP_CLIENT_HTTPCLIENT_MAX_REQUEST_SIZE);
  char buffer[_ARIA_C_HTTP_CLIENT_HTTPCLIENT_MAX_REQUEST_SIZE];
  snprintf(buffer, sizeof(buffer), "%s %s HTTP/1.1\n", request->method, request->path);
  strcat(request_text, buffer);
  iterate_over_hash(request->headers, request_text, buffer);
  return request_text;
}

HTTPResponse *send(HTTPRequest *request, char *host, int port) {
  HTTPResponse *response = malloc(sizeof(HTTPResponse));
  printf("%s", mount_http(request));
  free(request->headers);
  free(request);
  return response;
}