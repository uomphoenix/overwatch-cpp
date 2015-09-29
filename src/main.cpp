#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "include/LeptonCamera.h"
#include "include/LeptonCameraContainer.h"

#include "include/PiCameraContainer.h"

#include "include/AuthenticationClient.h"
#include "include/VideoFeedClient.h"
#include "include/SocketClient.h"

#include "include/PanSharpen.h"

#include "include/Exception.h"

#include "raspicam/raspicam_cv.h"

#define TEST_MODE true

#ifdef TEST_MODE
void test_sending();
void test_send_with_video();
void test_lepton();
void test_picam();
void test_grayscale_jpg();
void test_pansharpen();
#endif

int main(int argc, char *argv[])
{
#ifdef TEST_MODE
    //cv::namedWindow("Lepton", cv::WINDOW_AUTOSIZE);
    // Run a test function
    //test_lepton();
    //test_send_with_video();

    //test_grayscale_jpg();
    test_pansharpen();


#else
    // ACTUAL PROGRAM HERE


#endif
    return 0;
}

#ifdef TEST_MODE
void test_pansharpen()
{
    cv::namedWindow("test", CV_WINDOW_AUTOSIZE);
    PanSharpen ps;
    ps.read_from_disk(std::string("/home/dcandy/Downloads/imax_rgb.jpg"));
}

void test_sending()
{
    char *ident = (char *)malloc(128);
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
        throw;
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
    delete vclient;
}

void test_send_with_video()
{
    char *ident = (char *)malloc(128);
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
    catch (ConnectionError& ex)
    {
        std::cout << "Unable to connect to Firefly server" << std::endl;
        throw;
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

        LeptonCamera *lep = new LeptonCamera();
        LeptonCameraContainer *lpc = new LeptonCameraContainer(lep);

        #if HAVE_LEPTON
        lep->initLepton();
        #endif

        while (true)
        {
            lpc->getNextFrame();
            cv::Mat frame = lpc->getLatestFrame();
            vclient->send_frame(frame);

            std::vector<unsigned char> buf;
            std::vector<int> params;
            params.push_back(CV_IMWRITE_JPEG_QUALITY);
            params.push_back(80);

            cv::imencode(std::string(".jpeg"), frame, buf, params);

            cv::Mat dec = cv::imdecode(buf, -1);


            cv::imshow("Lepton", frame);
            cv::imshow("Lepton2", dec);

            cv::waitKey(1);

//            break;
        }
    }

    getchar();

    delete auth;
    delete vclient;
}

void test_lepton()
{
    LeptonCamera *lep = new LeptonCamera();
    #if HAVE_LEPTON
    lep->initLepton();
    #endif

    LeptonCameraContainer *lpc = new LeptonCameraContainer(lep);

    while (true)
    {
        lpc->getNextFrame();
        cv::Mat frame = lpc->getLatestFrame();

        //std::cout << "showing frame" << std::endl << tmp << std::endl;

        cv::imshow("Lepton", frame);

        cv::waitKey(1);
    }
}

void test_picam()
{
    cv::namedWindow("PiCam", cv::WINDOW_AUTOSIZE);

    raspicam::RaspiCam_Cv *picam = new raspicam::RaspiCam_Cv();

    std::cout << "Opening pi cam" << std::endl;

    if (!picam->open())
    {
        throw PiCamOpenError();
    }

    PiCameraContainer *pcc = new PiCameraContainer(picam);

    while (true)
    {
        std::cout << "getting frame" << std::endl;
        pcc->getNextFrame();
        cv::Mat frame = pcc->getLatestFrame();

        //std::cout << "showing frame" << std::endl << tmp << std::endl;

        cv::imshow("PiCam", frame);

        cv::waitKey(1);
    }
}

void test_grayscale_jpg()
{
    // Read file & convert it into JPEG.
    cv::VideoCapture cap("./video/lepton_6.avi");

    cv::Mat frame;
    cv::Mat gray;
    while (cap.read(frame))
    {
        std::vector<unsigned char> buf;
        std::vector<int> params;
        params.push_back(CV_IMWRITE_JPEG_QUALITY);
        params.push_back(80);

        cvtColor(frame, gray, CV_BGR2GRAY);

        cv::imencode(std::string(".jpg"), gray, buf, params);

        cv::Mat dec = cv::imdecode(buf, -1);


        cv::imshow("Lepton", frame);
        cv::imshow("Lepton2", dec);
        cv::waitKey(3);
    }
}
#endif
