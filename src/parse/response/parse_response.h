#ifndef _ARIA_HTTP_CLIENT_SRC_PARSE_RESPONSE_H
#define _ARIA_HTTP_CLIENT_SRC_PARSE_RESPONSE_H

#include "../../../lib/aria_data_structures/datastructures.h"

typedef struct HTTPResponse {
  char *body;
  HashTable *headers;
  int status;
} HTTPResponse;

HTTPResponse *parse_response(char *response);

#endif // _ARIA_HTTP_CLIENT_SRC_PARSE_RESPONSE_H