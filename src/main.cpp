#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"

#include "include/LeptonCamera.h"
#include "include/LeptonCameraContainer.h"

#include "include/AuthenticationClient.h"
#include "include/VideoFeedClient.h"
#include "include/SocketClient.h"

//#include "raspicam/raspicam_cv.h"


int main(int argc, char *argv[])
{
    /*char *ident = (char *)malloc(128);
    char *host = (char *)malloc(128);
    strncpy(ident, "TEST", 128);
    strncpy(host, "192.168.101.129", 128);
    int port = 56789;

    VideoFeedClient *vclient = NULL;
    AuthenticationClient *auth = new AuthenticationClient(host, port, std::string(ident));

    try
    {
        auth->authenticate();
    }
    catch (ConnectionError ex)
    {
        std::cout << "Unable to authenticate" << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "Unknown error" << std::endl;
        throw;
    }

    if (auth->is_authenticated())
    {
        vclient = new VideoFeedClient(auth->get_dyn_receiver_host(),
                                      auth->get_receiver_port(),
                                      auth->get_token());

        try
        {
            std::stringstream test;
            test << auth->get_token();
            test << '\x00';
            test << 1 << '\x00' << 1 << '\x00' << 1 << '\x00';
            test << "frame";

            std::string _test = test.str();

            char *to_send = (char *)malloc(_test.length()+1);
            memcpy(to_send, _test.c_str(), _test.length()+1);

            std::cout << "Sending frame... ";
            for (int i = 0; i < _test.length(); i++)
                std::cout << (int)_test.at(i) << " ";
            std::cout << " done" << std::endl;

            vclient->send_bytes(to_send, _test.length()+1);
            std::cout << "Frame apparently sent" << std::endl;
        }
        catch (...)
        {
            throw;
        }
    }

    getchar();

    delete auth;
    delete vclient;*/

    cv::namedWindow("Lepton", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("PiCam", cv::WINDOW_AUTOSIZE);

    LeptonCamera *lep = new LeptonCamera();
    LeptonCameraContainer *lpc = new LeptonCameraContainer(lep);

    #if HAVE_LEPTON
    lep->initLepton();
    #endif

    //raspicam::RaspiCam_Cv *picam = new raspicam::RaspiCam_Cv();
    //std::cout << "Opening pi cam" << std::endl;

    //if (!picam->open())
    //{
    //    return 0;
    //}


    while (true)
    {

        std::cout << "getting frame" << std::endl;
        lpc->getNextFrame();
        cv::Mat frame = lpc->getLatestFrame();

        //std::cout << "showing frame" << std::endl << tmp << std::endl;

        cv::imshow("Lepton", frame);
        //cv::imshow("PiCam", tmp);

        cv::waitKey(1);
        //break;
        //sleep(1);
    }

    /*cv::Mat test(LeptonCamera::FrameHeight, LeptonCamera::FrameWidth, CV_16UC1);
    unsigned short *p;

    for (int i = 0; i < test.rows; ++i)
    {
        p = test.ptr<unsigned short>(i);

        for (int j = 0; j < test.cols; ++j)
        {
            //p[j] = 5;
            test.data[i*test.step +j] = 5;

        }
    }

    std::cout << test << std::endl;*/

    return 0;
}
