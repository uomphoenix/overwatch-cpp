#ifndef CAMERACONTAINER_H
#define CAMERACONTAINER_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv/cv.h"

class CameraContainer
{
    public:
        CameraContainer();
        virtual ~CameraContainer();

        virtual void run();
        virtual Mat *getLatestFrame();

    protected:
        Mat *latest_frame;
    private:
};

#endif // CAMERACONTAINER_H
