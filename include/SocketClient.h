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
        virtual int send_bytes(char *, size_t) {};
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

class SocketBindError : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Cannot bind socket to host";
    }
};

class SocketSendError : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unable to send data on socket";
    }
};

#endif // SOCKETCLIENT_H
