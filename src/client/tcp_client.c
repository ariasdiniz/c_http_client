#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 65536

HTTPResponse *shttp(HTTPRequest *request, char *host, unsigned int port) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int read_size;
    HTTPResponse *response;

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

    strcpy(buffer, parse_request(request));
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        close(sock);
        return NULL;
    }

    read_size = recv(sock, buffer, BUFFER_SIZE, 0);
    if (read_size < 0) {
        close(sock);
        return NULL;
    }

    // TODO: Parse response to HTTPResponse

    close(sock);
    return response;
}
