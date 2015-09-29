#ifndef PANSHARPEN_H
#define PANSHARPEN_H

#include <stdlib.h>
#include <string>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

class PanSharpen
{
public:
    PanSharpen();
    void read_from_disk(std::string filename);

};

#endif // PANSHARPEN_H
