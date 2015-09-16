#ifndef PICAMERACONTAINER_H
#define PICAMERACONTAINER_H

#include "include/CameraContainer.h"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv/cv.h"

#include "raspicam/raspicam_cv.h"

class PiCameraContainer : public CameraContainer
{
    public:
        PiCameraContainer(raspicam::RaspiCam_Cv *);
        virtual ~PiCameraContainer();

        virtual cv::Mat *getLatestFrame();
    protected:
        virtual void getNextFrame();

    private:
        raspicam::RaspiCam_Cv *camera;
};

#endif // PICAMERACONTAINER_H
