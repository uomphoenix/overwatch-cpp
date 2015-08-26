#ifndef VIDEOFEEDCLIENT_H
#define VIDEOFEEDCLIENT_H

#include <SocketClient.h>


class VideoFeedClient : public SocketClient
{
    public:
        VideoFeedClient(char *host, int port);
        virtual ~VideoFeedClient();
    protected:
    private:
};

#endif // VIDEOFEEDCLIENT_H
