#include <iostream>

#include <cmath>

#include "opencv2/imgcodecs.hpp"

#include "include/VideoFeedClient.h"

VideoFeedClient::VideoFeedClient(char *host, int port, std::string token)
    : SocketClient(host, port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Bind the socket to a random port
    struct sockaddr_in bind_addr;
    memset((char *)&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind_addr.sin_port = htons(0);

    if (bind(sockfd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0)
    {
        throw SocketBindError();
        connected = true;
    }

    this->token = token;
    seq_id = 0;
}

VideoFeedClient::VideoFeedClient(struct sockaddr_in *receiver_addr)
{
    memcpy(&serv_addr, receiver_addr, sizeof(*receiver_addr));

    //host = ntohl(receiver_addr->sin_addr.s_addr);
    //port = ntohs(receiver_addr->sin_port);
}

VideoFeedClient::~VideoFeedClient()
{
    if (connected || sockfd >= 0)
    {
        close(sockfd);
        sockfd = -1;
    }
}

int VideoFeedClient::connect_sock()
{
    return 0;
}

int VideoFeedClient::send_bytes(char *bytes, size_t len)
{
    int sent = 0;
    if ((sent = sendto(sockfd, bytes, len, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        throw SocketSendError();
    }

    return sent;
}

void VideoFeedClient::send_frame(cv::Mat& frame)
{
    // We take a cv::Mat and encode it into JPEG. Then we send this frame in
    // chunks to the receiver. We want this to be as fast as possible...
    std::vector<unsigned char> buf;
    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(80);

    cv::imencode(std::string(".jpg"), frame, buf, params);

//    for (unsigned int i = 0; i < buf.size(); i++)
//        std::cout << (int)buf[i] << " ";
//    std::cout << std::endl;

    // FORMAT:
    // <challenge>\x00<seq num>\x00<max fragments>\x00<fragment num>\x00<frame>\x00
    std::stringstream frame_builder;

    int num_fragments = (int)ceil(1.*buf.size()/MAX_FRAGMENT_SIZE);

    // Buffer on the stack for optimisation!
    char frame_buffer[MAX_FRAGMENT_SIZE];
    unsigned int curr_index = 0, end_index = MAX_FRAGMENT_SIZE;

    // Increment the sequence ID here. This needs to be incremented for every
    // frame sent by this client.
    seq_id++;

    for (int i = 0; i < num_fragments; i++)
    {
        // Ensure we aren't over running the JPEG buffer
        if (end_index > buf.size())
            end_index = buf.size();

        // Read from the current index up until the end index. To get the
        // length of the data we want, we take the difference between the end
        // and current index. eg. at pos 10, end index is 20, we want to read
        // 10 bytes starting from the current position.
        memcpy(frame_buffer, buf.data()+curr_index, end_index-curr_index);

//        for (unsigned int j = 0; j < end_index-curr_index; j++)
//            std::cout << j << ":" << (int)frame_buffer[j] << " ";
//        std::cout << std::endl;

        std::cout << "Frame " << seq_id << " index range " << curr_index;
        std::cout << " to " << end_index << ", fragment: " << i << std::endl;

        frame_builder << token;
        frame_builder << '\x00';
        frame_builder << seq_id << '\x00';
        frame_builder << num_fragments << '\x00';
        frame_builder << i << '\x00';
        frame_builder << std::string(frame_buffer, end_index-curr_index);

        std::string _bytes = frame_builder.str();

        size_t send_len = _bytes.length()+1;
        char *to_send = (char *)malloc(send_len);
        memcpy(to_send, _bytes.c_str(), send_len);

        std::cout << "Sending frame... ";
        for (unsigned int j = 0; j < send_len; j++)
            std::cout << (int)to_send[j] << " ";
        std::cout << " done" << std::endl;

        send_bytes(to_send, send_len);
        std::cout << "Frame apparently sent" << std::endl;

        free(to_send);

        // Clear the buffer for the next fragment
        frame_builder.str(std::string());
        frame_builder.clear();

        curr_index = end_index;
        end_index += MAX_FRAGMENT_SIZE;
    }
}

/*
// We take a cv::Mat and encode it into JPEG. Then we send this frame in
    // chunks to the receiver. We want this to be as fast as possible...
    cv::Mat encoded(frame);



    std::vector<unsigned char> buf;
    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    cv::imencode(std::string(".jpg"), encoded, buf, params);

    for (unsigned int i = 0; i < buf.size(); i++)
        std::cout << (int)buf[i] << " ";
    std::cout << std::endl;

    // FORMAT:
    // <challenge>\x00<seq num>\x00<max fragments>\x00<fragment num>\x00<frame>\x00
    std::stringstream frame_builder;

    unsigned int frame_size = encoded.total()*encoded.elemSize();
    int num_fragments = (int)ceil(1.*frame_size/MAX_FRAGMENT_SIZE);


    // Buffer on the stack for optimisation!
    char frame_buffer[MAX_FRAGMENT_SIZE];
    unsigned int curr_index = 0, end_index = MAX_FRAGMENT_SIZE;

    // Increment the sequence ID here. This needs to be incremented for every
    // frame sent by this client.
    seq_id++;

    for (int i = 0; i < num_fragments; i++)
    {
        // Ensure we aren't over running the frame's data. Remember C strings
        // are 0 indexed.
        if (end_index >= frame_size)
            end_index = frame_size-1;

        // Read from the current index up until the end index. To get the
        // length of the data we want, we take the difference between the end
        // and current index. eg. at pos 10, end index is 20, we want to read
        // 10 bytes starting from the current position.
        memcpy(frame_buffer, encoded.data+curr_index, end_index-curr_index);

        // For the sake of speed we'll just put a single terminator at the end
        // of our buffer's data, rather than zeroing the entire rest of the
        // buffer.
//        frame_buffer[end_index-curr_index] = '\x00';
//        for (unsigned int j = 0; j < end_index-curr_index; j++)
//            std::cout << j << ":" << (int)frame_buffer[j] << " ";
//        std::cout << std::endl;

        std::cout << "Frame " << seq_id << " index range " << curr_index;
        std::cout << " to " << end_index << ", fragment: " << i << std::endl;

        frame_builder << token;
        frame_builder << '\x00';
        frame_builder << seq_id << '\x00';
        frame_builder << num_fragments << '\x00';
        frame_builder << i << '\x00';
        frame_builder << std::string(frame_buffer, end_index-curr_index) << '\x00';

        std::string _bytes = frame_builder.str();

        char *to_send = (char *)malloc(_bytes.length()+1);
        memcpy(to_send, _bytes.c_str(), _bytes.length()+1);

        std::cout << "Sending frame... ";
        for (unsigned int j = 0; j < _bytes.length(); j++)
            std::cout << (int)_bytes.at(j) << " ";
        std::cout << " done" << std::endl;

        send_bytes(to_send, _bytes.length()+1);
        std::cout << "Frame apparently sent" << std::endl;

        free(to_send);

        // Clear the buffer for the next fragment
        frame_builder.str(std::string());
        frame_builder.clear();

        curr_index = end_index;
        end_index += MAX_FRAGMENT_SIZE;
    }
    */
