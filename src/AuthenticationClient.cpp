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
#include <sstream>
#include <iomanip>

#include "include/AuthenticationClient.h"
#include "include/SocketClient.h"
#include "include/util.h"

AuthenticationClient::AuthenticationClient(char *host, int port,
                                           std::string identifier)
    : SocketClient(host, port)
{
    authenticated = false;
    this->identifier = identifier;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

AuthenticationClient::~AuthenticationClient()
{
    if (connected || sockfd >= 0)
    {
        close(sockfd);
        sockfd = -1;
    }
}

int AuthenticationClient::connect_sock()
{
    if (connected)
    {
        return -1;
    }

    int res;
    if ((res = connect(sockfd, (struct sockaddr *) &serv_addr,
                       sizeof(serv_addr))) == 0)
    {
        connected = true;
    }

    if (res < 0)
    {
        throw ConnectionError();
    }

    return res;
}

int AuthenticationClient::send_bytes(char *bytes, size_t len)
{
    if (!connected)
    {
        return -1;
    }

    int rtn_size = 0;
    size_t bytes_sent = 0;

    while (bytes_sent < len)
    {
        rtn_size = write(sockfd, bytes, len);

        if (rtn_size == -1)
        {
            std::cout << "Error sending data via socket" << std::endl;
            throw SocketSendError();
        }

        bytes_sent += (unsigned int)rtn_size;
    }

    return bytes_sent;
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
    request[0] = '\x01';
    request[1] = '\x00';
    strncpy(request+2, identifier.c_str(), identifier.length());
    request[request_len] = '\x00';

    send_bytes(request, request_len);
    std::cout << "ident: " << identifier << std::endl;

    std::cout << "Sent auth request: ";
    for (unsigned int i = 0; i < request_len; i++)
        std::cout << (unsigned int)request[i] << " ";
    std::cout << std::endl;

    free(request);

    // Get the response
    char *buffer = (char *)malloc(AUTH_RESPONSE_LEN);
    int response_len = read_bytes(buffer, AUTH_RESPONSE_LEN);

    // Note that we force string to read response_len bytes into the string,
    // otherwise it'll stop at the first \x00
    std::string response = std::string(buffer, response_len);
    std::cout << response << std::endl;

    free(buffer);

    std::vector<std::string> tokenized = util::split(response, '\x00');

    if (!tokenized.empty())
    {
        for (unsigned int i = 0; i < tokenized.size(); i++)
            std::cout << "tokenized at i = " << i << ": " << tokenized.at(i) << std::endl;
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
        connected = false;

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

char *AuthenticationClient::get_dyn_receiver_host()
{
    char *host = (char *)malloc(receiver_host.length()+1);
    strncpy(host, receiver_host.c_str(), receiver_host.length()+1);

    return host;
}

int AuthenticationClient::get_receiver_port()
{
    return receiver_port;
}
