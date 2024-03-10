#include "../../include/httpclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

HTTPResponse *shttp(HTTPRequest *request, char *host, unsigned int port, int use_ssl) {
  int sock;
  struct addrinfo hints, *result, *rp;
  char buffer[__BUFFER_SIZE];
  char *parsed_request;
  int read_size;
  HTTPResponse *response = NULL;

  // Initialize buffer with null characters
  memset(buffer, '\0', __BUFFER_SIZE);

  // Set up hints structure for getaddrinfo
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; // Use IPv4
  hints.ai_socktype = SOCK_STREAM;

  // Resolve the server's IP address or hostname
  if (getaddrinfo(host, NULL, &hints, &result) != 0) {
    return NULL;
  }

  // Iterate over the available addresses and connect to the first successful one
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    // Create a TCP socket
    sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sock == -1) {
      continue;
    }

    // Set up server address structure
    struct sockaddr_in *server_addr = (struct sockaddr_in *)rp->ai_addr;
    server_addr->sin_port = htons(port);

    // Connect to the server
    if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1) {
      break; // Success
    }

    close(sock);
  }

  // Free the address information structure
  freeaddrinfo(result);

  // Check if a successful connection was established
  if (rp == NULL) {
    return NULL;
  }

  if (use_ssl) {
    // Initialize SSL
    SSL_library_init();
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    if (!ctx) {
      close(sock);
      return NULL;
    }

    // Create SSL connection
    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
      close(sock);
      SSL_CTX_free(ctx);
      return NULL;
    }

    // Set up the SSL connection
    if (SSL_set_fd(ssl, sock) == 0) {
      close(sock);
      SSL_free(ssl);
      SSL_CTX_free(ctx);
      return NULL;
    }

    // Perform SSL handshake
    if (SSL_connect(ssl) != 1) {
      close(sock);
      SSL_free(ssl);
      SSL_CTX_free(ctx);
      return NULL;
    }

    // Parse the HTTP request and store it in buffer
    parsed_request = parse_request(request);
    strcpy(buffer, parsed_request);
    free(parsed_request);

    // Send the HTTP request over SSL
    if (SSL_write(ssl, buffer, strlen(buffer)) < 0) {
      close(sock);
      SSL_free(ssl);
      SSL_CTX_free(ctx);
      return NULL;
    }

    // Receive the HTTP response over SSL
    read_size = SSL_read(ssl, buffer, __BUFFER_SIZE - 1);
    if (read_size < 0) {
      close(sock);
      SSL_free(ssl);
      SSL_CTX_free(ctx);
      return NULL;
    }

    // Parse the HTTP response and store it in the response structure
    response = parse_response(buffer);

    // Close SSL connection
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
  } else {
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
  }

  // Close the socket
  close(sock);
  return response;
}
