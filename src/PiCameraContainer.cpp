#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv/cv.h"

#include "PiCameraContainer.h"

PiCameraContainer::PiCameraContainer()
{
    //ctor
}

PiCameraContainer::~PiCameraContainer()
{
    //dtor
}

void PiCameraContainer::run()
{

}

cv::Mat *PiCameraContainer::getLatestFrame()
{
    return NULL;
}
