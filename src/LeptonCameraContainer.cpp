#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "include/LeptonCameraContainer.h"

LeptonCameraContainer::LeptonCameraContainer(LeptonCamera *camera)
    : CameraContainer()
{
    this->camera = camera;

    latest_frame = cv::Mat(LeptonCamera::FrameHeight, LeptonCamera::FrameWidth, CV_16UC1);
}

LeptonCameraContainer::~LeptonCameraContainer()
{

}

/**
 * Gets the next available frame from the camera, and moves the last frame to
 * the previous frame variable.
 */
void LeptonCameraContainer::getNextFrame()
{
    // Note that the data in latest_frame is never updated if getFrame returns
    // false, so we're just returning the previous frame.
    if (camera->getFrame(latest_frame))
        latest_ftime = std::time(NULL);
}

cv::Mat LeptonCameraContainer::getLatestFrame()
{
    // We need to return a newly allocated frame buffer in this method,
    // otherwise the thread running the camera capture will destroy
    // the data in the current pointer...

    return latest_frame.clone();
}
