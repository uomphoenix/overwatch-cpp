#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "include/AuthenticationClient.h"
#include "include/VideoFeedClient.h"
#include "include/SocketClient.h"

int main(int argc, char *argv[])
{
    std::cout << "hello" << std::endl;

    char *ident = "TEST";
    char *host = "192.168.101.129";
    int port = 56789;

    AuthenticationClient *auth = new AuthenticationClient(host, port, ident);

    try
    {
        auth->authenticate();
    }
    catch (ConnectionError ex)
    {
        std::cout << "Unable to authenticate" << std::endl;
        return 1;
    }

    if (auth->is_authenticated())
    {
        VideoFeedClient *vclient = new VideoFeedClient(auth->get_receiver_host().c_str(),
                                                       auth->get_receiver_port(),
                                                       auth->get_token());
    }



    getchar();
    return 0;
}
