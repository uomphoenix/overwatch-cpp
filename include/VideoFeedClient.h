#ifndef VIDEOFEEDCLIENT_H
#define VIDEOFEEDCLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "include/SocketClient.h"

#define MAX_FRAGMENT_SIZE 4096

class VideoFeedClient : public SocketClient
{
    public:
        VideoFeedClient(char *host, int port, std::string token);
        VideoFeedClient(struct sockaddr_in *receiver_addr);

        virtual int connect_sock();
        int send_bytes(char *bytes, size_t len);

        void send_frame(cv::Mat& frame);

        virtual ~VideoFeedClient();
    protected:
    private:
        std::string token;
        int seq_id;
};

#endif // VIDEOFEEDCLIENT_H
