#ifndef PANSHARPEN_H
#define PANSHARPEN_H

#include <stdlib.h>
#include <string>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/ximgproc.hpp"

class PanSharpen
{
public:
    PanSharpen();
    void read_from_disk(std::string filename);

    void sharpen(cv::Mat& thermal, cv::Mat& visual, cv::Mat& dst);

protected:
    void std_deviation_filter(cv::Mat& input, Mat& output);

private:
    int radius;
    double epsilon;

};

#endif // PANSHARPEN_H
