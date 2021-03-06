#ifndef PICAMERACONTAINER_H
#define PICAMERACONTAINER_H

#include "include/CameraContainer.h"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "raspicam/raspicam_cv.h"

#define PICAM_FRAME_WIDTH 640
#define PICAM_FRAME_HEIGHT 480

class PiCameraContainer : public CameraContainer
{
    public:
        PiCameraContainer(raspicam::RaspiCam_Cv *);
        virtual ~PiCameraContainer();

        virtual cv::Mat getLatestFrame();
        virtual void getNextFrame();
    protected:

    private:
        raspicam::RaspiCam_Cv *camera;
};

#endif // PICAMERACONTAINER_H
