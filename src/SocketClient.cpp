#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>

#include "include/SocketClient.h"

SocketClient::SocketClient(char *host, int port)
{
    this->host = host;
    this->port = port;

    server = gethostbyname(host);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);

    connected = false;
    sockfd = -1;
}

SocketClient::SocketClient()
{

}

SocketClient::~SocketClient()
{
    if (connected || sockfd > 0)
        close(sockfd);

    if (host != NULL)
    {
        free(host);
        host = NULL;
    }
}

int SocketClient::read_bytes(char *buf, size_t len)
{
    int num_read = read(sockfd, buf, len);
    if (num_read == -1)
    {
        perror("Error reading from socket");
        throw SocketReadError();
    }

    return num_read;
}

int SocketClient::connect_sock()
{
    return 0;
}

int SocketClient::send_bytes(char *, size_t)
{
    return 0;
}

