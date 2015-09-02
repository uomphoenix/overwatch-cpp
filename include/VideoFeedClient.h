#ifndef VIDEOFEEDCLIENT_H
#define VIDEOFEEDCLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>

#include <string>

#include "include/SocketClient.h"


class VideoFeedClient : public SocketClient
{
    public:
        VideoFeedClient(char *host, int port, std::string token);
        //VideoFeedClient(struct sockaddr_in *receiver_addr);


        int connect_sock();
        virtual ~VideoFeedClient();
    protected:
    private:
};

#endif // VIDEOFEEDCLIENT_H
