#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>

#include <exception>

class SocketClient
{
    public:
        SocketClient(char *host, int port);
        SocketClient();
        virtual ~SocketClient();

        virtual int connect_sock() {};
        int send_bytes(char *bytes, size_t len);
        int read_bytes(char *buf, size_t len);

    protected:
        struct sockaddr_in serv_addr;
        struct hostent *server;

        int sockfd;
        bool connected;

        char *host;
        int port;

    private:
};

class ConnectionError : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unable to establish socket connection";
    }
};

#endif // SOCKETCLIENT_H
