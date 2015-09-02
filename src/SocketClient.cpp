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
}

SocketClient::SocketClient()
{

}

SocketClient::~SocketClient()
{
    if (connected || sockfd > 0)
        close(sockfd);
}

int SocketClient::send_bytes(char *bytes, size_t len)
{
    if (!connected)
    {
        return -1;
    }

    int rtn_size, bytes_sent = 0;

    while (bytes_sent < len)
    {
        rtn_size = write(sockfd, bytes, len);

        if (rtn_size == -1)
        {
            std::cout << "Error sending data via socket" << std::endl;
            return -1;
        }

        bytes_sent += rtn_size;
    }

    return bytes_sent;
}

int SocketClient::read_bytes(char *buf, size_t len)
{
    int num_read = read(sockfd, buf, len);
    if (num_read == -1)
    {
        perror("Error reading from socket");
    }
}

