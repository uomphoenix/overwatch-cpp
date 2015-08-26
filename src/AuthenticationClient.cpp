#include "AuthenticationClient.h"

AuthenticationClient::AuthenticationClient(char *host, int port)
: SocketClient(char *host, int port)
{

}

AuthenticationClient::~AuthenticationClient()
{
    //dtor
}

int AuthenticationClient::connect_sock()
{
    if (connected)
    {
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int res;
    if ((res = connect(sockfd, (struct sockaddr *) &serv_addr,
                       sizeof(serv_addr))) == 0)
    {
        connected = true;
    }

    return res;
}
