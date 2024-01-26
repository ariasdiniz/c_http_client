#ifndef _ARIA_HTTP_CLIENT_SRC_HTTPCLIENT_H
#define _ARIA_HTTP_CLIENT_SRC_HTTPCLIENT_H

#include "../lib/aria_data_structures/src/hash/hash.h"

typedef struct HTTPRequest {
  char *url;
  HashTable *params;
  HashTable *headers;
  char *body;
} HTTPRequest;

typedef struct HTTPResponse {
  unsigned int status;
  HashTable *params;
  HashTable *headers;
  char *body;
} HTTPResponse;

HTTPResponse *send(HTTPRequest *request);

#endif
