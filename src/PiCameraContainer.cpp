
#include "include/PiCameraContainer.h"

PiCameraContainer::PiCameraContainer(raspicam::RaspiCam_Cv *camera)
    : CameraContainer()
{
    this->camera = camera;
}

PiCameraContainer::~PiCameraContainer()
{
    //dtor
}

void PiCameraContainer::getNextFrame()
{
    camera->retrieve(latest_frame);
}

cv::Mat PiCameraContainer::getLatestFrame()
{
    return latest_frame.clone();
}
