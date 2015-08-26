#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <stdlib.h>

class SocketClient
{
    public:
        SocketClient(char *host, int port);
        virtual ~SocketClient();

        virtual int connect_sock();
        int send_bytes(char *bytes, size_t len);

    protected:
        struct sockaddr_in serv_addr;
        struct hostent *server;

        int sockfd;
        bool connected;

        char *host;
        int port;

    private:
};

#endif // SOCKETCLIENT_H
