#ifndef LEPTONCAMERACONTAINER_H
#define LEPTONCAMERACONTAINER_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "include/CameraContainer.h"

#include "include/LeptonCamera.h"

class LeptonCameraContainer : public CameraContainer
{
    public:
        LeptonCameraContainer(LeptonCamera *);
        virtual ~LeptonCameraContainer();

        virtual cv::Mat getLatestFrame();
        virtual void getNextFrame();
    protected:
        cv::Mat latest_frame;

    private:
        LeptonCamera *camera;
};

#endif // LEPTONCAMERACONTAINER_H
