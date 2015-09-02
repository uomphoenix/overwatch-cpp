#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string>

#include "include/SocketClient.h"
#include "include/VideoFeedClient.h"

VideoFeedClient::VideoFeedClient(char *host, int port, std::string token)
    : SocketClient(host, port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Bind the socket to a random port
    struct sockaddr_in bind_addr;
    memset((char *)&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind_addr.sin_port = htons(0);

    if (bind(sockfd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0)
    {
        throw new SocketBindError();
    }
}

/*VideoFeedClient::VideoFeedClient(struct sockaddr_in *receiver_addr)
{
    memcpy(serv_addr, receiver_addr, sizeof(*receiver_addr));

    host = receiver_addr->sin_addr.sin_addr;
    port = ntohs(receiver_addr->sin_port);
}*/

VideoFeedClient::~VideoFeedClient()
{

}

int VideoFeedClient::connect_sock()
{

}

int VideoFeedClient::send_bytes(char *bytes, size_t len)
{
    int sent = 0;
    if ((sent = sendto(sockfd, bytes, len, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        throw new SocketSendError();
    }

    return sent;
}
