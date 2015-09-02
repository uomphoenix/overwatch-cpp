#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string>

#include "include/SocketClient.h"
#include "include/VideoFeedClient.h"

VideoFeedClient::VideoFeedClient(char *host, int port, std::string token)
    : SocketClient(host, port)
{
    //ctor
}

/*VideoFeedClient::VideoFeedClient(struct sockaddr_in *receiver_addr)
{
    memcpy(serv_addr, receiver_addr, sizeof(*receiver_addr));

    host = receiver_addr->sin_addr.sin_addr;
    port = ntohs(receiver_addr->sin_port);
}*/

VideoFeedClient::~VideoFeedClient()
{
    //dtor
}

int VideoFeedClient::connect_sock()
{
    if (connected)
    {
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    int res;
    if ((res = connect(sockfd, (struct sockaddr *) &serv_addr,
                       sizeof(serv_addr))) == 0)
    {
        connected = true;
    }

    if (res < 0)
    {
        throw new ConnectionError();
    }

    return res;
}
