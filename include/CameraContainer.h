#ifndef CAMERACONTAINER_H
#define CAMERACONTAINER_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include <ctime>

class CameraContainer
{
    public:
        CameraContainer();
        virtual ~CameraContainer();

        void run();
        void stop();
        virtual cv::Mat getLatestFrame() {};

    protected:
        virtual void getNextFrame() {};

        cv::Mat *prev_frame;
        std::time_t prev_ftime;

        cv::Mat *latest_frame;
        std::time_t latest_ftime;

        bool running;
    private:
};

#endif // CAMERACONTAINER_H
