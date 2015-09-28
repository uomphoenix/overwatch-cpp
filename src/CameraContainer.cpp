#include "CameraContainer.h"

CameraContainer::CameraContainer()
{
    running = false;

    latest_ftime = 0;
}

CameraContainer::~CameraContainer()
{
    //dtor
}

void CameraContainer::run()
{
    if (running)
        return;

    running = true;

    while (running)
    {
        getNextFrame();
    }
}

void CameraContainer::stop()
{
    running = false;
}
