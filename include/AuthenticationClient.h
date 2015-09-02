#ifndef AUTHENTICATIONCLIENT_H
#define AUTHENTICATIONCLIENT_H

#define AUTH_RESPONSE_LEN 128

#include <stdlib.h>
#include <string>

#include "include/SocketClient.h"

class AuthenticationClient : public SocketClient
{
    public:
        AuthenticationClient(char *host, int port, std::string identifier);

        int connect_sock();
        bool authenticate();

        struct sockaddr_in *get_receiver_addr();
        bool is_authenticated();
        std::string get_token();
        std::string get_receiver_host();
        int get_receiver_port();

        virtual ~AuthenticationClient();
    protected:
    private:
        std::string identifier;
        std::string token;
        std::string receiver_host;
        int receiver_port;

        struct sockaddr_in receiver_addr;

        bool authenticated;
};

#endif // AUTHENTICATIONCLIENT_H
