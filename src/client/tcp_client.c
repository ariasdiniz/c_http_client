#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 65536

int shttp(HTTPRequest *request, char *host, unsigned int port) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int read_size;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(sock);
        return -1;
    }

    strcpy(buffer, parse_request(request));
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("Send failed");
        close(sock);
        return -1;
    }

    read_size = recv(sock, buffer, BUFFER_SIZE, 0);
    if (read_size < 0) {
        perror("Receive failed");
        close(sock);
        return -1;
    }

    printf("%s\n", buffer);

    close(sock);
    return 0;
}
