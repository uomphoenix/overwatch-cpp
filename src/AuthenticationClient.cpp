#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string>
#include <cstring>
#include <iostream>

#include "include/AuthenticationClient.h"
#include "include/SocketClient.h"
#include "include/util.h"

AuthenticationClient::AuthenticationClient(char *host, int port,
                                           std::string identifier)
    : SocketClient(host, port)
{
    authenticated = false;
    this->identifier = identifier;
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

    if (res < 0)
    {
        throw new ConnectionError();
    }

    return res;
}

bool AuthenticationClient::authenticate()
{
    if (authenticated)
        return true;

    if (!connected)
    {
        connect_sock();
    }

    /*
    Construct the authentication request. Authentication requests are in the
    format \x01\x00<identifier>\x00
    */
    size_t request_len = identifier.length() + 3;
    char *request = (char *)malloc(request_len);

    snprintf(request, request_len, "\x01\x00%s", identifier.c_str());
    request[request_len] = '\x00';

    int sent = send_bytes(request, request_len);

    std::cout << "Sent auth request " << request << std::endl;

    free(request);

    // Get the response
    char *buffer = (char *)malloc(AUTH_RESPONSE_LEN);
    int response_len = read_bytes(buffer, AUTH_RESPONSE_LEN);

    std::string response = std::string(buffer);

    free(buffer);

    std::vector<std::string> tokenized = util::split(response, '\x00');

    if (!tokenized.empty())
    {
        token = tokenized.at(1);
        receiver_host = tokenized.at(2);
        receiver_port = atoi(tokenized.at(3).c_str());

        struct hostent *recv_host = gethostbyname(receiver_host.c_str());

        bzero((char *) &receiver_addr, sizeof(receiver_addr));

        receiver_addr.sin_family = AF_INET;
        bcopy((char *)recv_host->h_addr,
              (char *) &receiver_addr.sin_addr.s_addr,
              server->h_length);
        receiver_addr.sin_port = htons(port);

        authenticated = true;

        close(sockfd);
        sockfd = -1;

        return true;
    }

    return false;
}

struct sockaddr_in* AuthenticationClient::get_receiver_addr()
{
    return &receiver_addr;
}

bool AuthenticationClient::is_authenticated()
{
    return authenticated;
}

std::string AuthenticationClient::get_token()
{
    return token;
}

std::string AuthenticationClient::get_receiver_host()
{
    return receiver_host;
}

int AuthenticationClient::get_receiver_port()
{
    return receiver_port;
}
