#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define __BUFFER_SIZE 65536

HTTPResponse *shttp(HTTPRequest *request, char *host, unsigned int port) {
  int sock;
  struct sockaddr_in server_addr;
  char buffer[__BUFFER_SIZE];
  char *parsed_request;
  int read_size;
  HTTPResponse *response = NULL;
  memset(buffer, '\0', __BUFFER_SIZE);

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    return NULL;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(host);

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    close(sock);
    return NULL;
  }

  parsed_request = parse_request(request);
  strcpy(buffer, parsed_request);
  free(parsed_request);
  if (send(sock, buffer, strlen(buffer), 0) < 0) {
    close(sock);
    return NULL;
  }

  read_size = recv(sock, buffer, __BUFFER_SIZE - 1, 0);
  if (read_size < 0) {
    close(sock);
    return NULL;
  }

  response = parse_response(buffer);

  close(sock);
  return response;
}
