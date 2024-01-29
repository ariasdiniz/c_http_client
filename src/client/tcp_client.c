#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define __BUFFER_SIZE 65536

/**
 * @brief Sends an HTTP request to a specified server and port using a TCP socket.
 *
 * @param request The HTTPRequest structure containing the request information.
 * @param host The server's IP address or hostname.
 * @param port The port number on which the server is listening.
 * @return An HTTPResponse structure containing the parsed response, or NULL on error.
 */
HTTPResponse *shttp(HTTPRequest *request, char *host, unsigned int port) {
  int sock;
  struct sockaddr_in server_addr;
  char buffer[__BUFFER_SIZE];
  char *parsed_request;
  int read_size;
  HTTPResponse *response = NULL;

  // Initialize buffer with null characters
  memset(buffer, '\0', __BUFFER_SIZE);

  // Create a TCP socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    return NULL;
  }

  // Set up server address structure
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(host);

  // Connect to the server
  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    close(sock);
    return NULL;
  }

  // Parse the HTTP request and store it in buffer
  parsed_request = parse_request(request);
  strcpy(buffer, parsed_request);
  free(parsed_request);

  // Send the HTTP request to the server
  if (send(sock, buffer, strlen(buffer), 0) < 0) {
    close(sock);
    return NULL;
  }

  // Receive the HTTP response from the server
  read_size = recv(sock, buffer, __BUFFER_SIZE - 1, 0);
  if (read_size < 0) {
    close(sock);
    return NULL;
  }

  // Parse the HTTP response and store it in the response structure
  response = parse_response(buffer);

  // Close the socket
  close(sock);
  return response;
}
