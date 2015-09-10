#ifndef PICAMERACONTAINER_H
#define PICAMERACONTAINER_H

#include <CameraContainer.h>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv/cv.h"

class PiCameraContainer : public CameraContainer
{
    public:
        PiCameraContainer();
        virtual ~PiCameraContainer();
    protected:
    private:
};

#endif // PICAMERACONTAINER_H
