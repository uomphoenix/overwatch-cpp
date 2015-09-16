#ifndef LEPTONCAMERACONTAINER_H
#define LEPTONCAMERACONTAINER_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv/cv.h"

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


    private:
        LeptonCamera *camera;
};

#endif // LEPTONCAMERACONTAINER_H
