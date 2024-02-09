#ifndef _ARIA_HTTP_CLIENT_SRC_PARSE_RESPONSE_H
#define _ARIA_HTTP_CLIENT_SRC_PARSE_RESPONSE_H

#include "../../../lib/aria_data_structures/include/datastructures.h"

typedef struct HTTPResponse {
  char *body;
  LinkedList *headers;
  int status;
} HTTPResponse;

HTTPResponse *parse_response(char *response);
int destroy_response(HTTPResponse *response);

#endif // _ARIA_HTTP_CLIENT_SRC_PARSE_RESPONSE_H
