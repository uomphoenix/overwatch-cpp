
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

void PiCameraContainer::run()
{

}

void PiCameraContainer::getLatestFrame(cv::Mat *)
{
    return NULL;
}
