#ifndef AUTHENTICATIONCLIENT_H
#define AUTHENTICATIONCLIENT_H

#include "SocketClient.h


class AuthenticationClient : public SocketClient
{
    public:
        AuthenticationClient(char *host, int port);
        int connect_sock();
        virtual ~AuthenticationClient();
    protected:
    private:
};

#endif // AUTHENTICATIONCLIENT_H
