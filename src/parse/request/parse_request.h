#ifndef _ARIA_HTTP_CLIENT_SRC_HTTPCLIENT_H
#define _ARIA_HTTP_CLIENT_SRC_HTTPCLIENT_H

#include "../lib/aria_data_structures/src/hash/hash.h"
#include "../lib/aria_data_structures/src/linkedlist/list.h"

typedef struct HTTPRequest {
  char *method;
  char *path;
  HashTable *headers;
  char *body;
} HTTPRequest;

typedef struct HTTPResponse {
  unsigned int status;
  HashTable *headers;
  char *body;
} HTTPResponse;

char *parse_request(HTTPRequest *request, char *host, int port);

#endif
