#include "VideoFeedClient.h"

VideoFeedClient::VideoFeedClient(char *host, int port)
: SocketClient(char *host, int port)
{
    //ctor
}

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

    return res;
}
