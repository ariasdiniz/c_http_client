#ifndef _ARIA_HTTP_CLIENT_SRC_TCP_CLIENT_H
#define _ARIA_HTTP_CLIENT_SRC_TCP_CLIENT_H

#include "../parse/request/parse_request.h"
#include "../parse/response/parse_response.h"

HTTPResponse *send_request(HTTPRequest *request, char *host, int port);

#endif // _ARIA_HTTP_CLIENT_SRC_TCP_CLIENT_H