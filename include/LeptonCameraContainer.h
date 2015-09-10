#ifndef LEPTONCAMERACONTAINER_H
#define LEPTONCAMERACONTAINER_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv/cv.h"

#include <CameraContainer.h>


class LeptonCameraContainer : public CameraContainer
{
    public:
        LeptonCameraContainer();
        virtual ~LeptonCameraContainer();
    protected:
    private:
};

#endif // LEPTONCAMERACONTAINER_H
