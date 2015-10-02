#include "include/PanSharpen.h"

using namespace cv;

PanSharpen::PanSharpen()
{
}

void PanSharpen::read_from_disk(std::string filename)
{

    std::string rgb_path = "/home/dcandy/Downloads/" + filename + "_rgb.jpg";
    std::string thermal_path = "/home/dcandy/Downloads/" + filename + "_thermal_smoothed.jpg";


    Mat rgb_image = imread(rgb_path);
    Mat thermal_image = imread(thermal_path);


    if (rgb_image.empty() || thermal_image.empty())
    {
        std::cout << "invalid file " << filename << std::endl;
        return;
    }



    std::cout << thermal_image.size() << std::endl;



    //imshow("test rgb", rgb_image);
    //imshow("test thermal", thermal_image);


    Mat output = thermal_image;


    //guided filter syntax
    //C++: void guidedFilter(InputArray guide, InputArray src, OutputArray dst, int radius, double eps, int dDepth=-1)

    //ximgproc::guidedFilter(rgb_image,thermal_image,output,10,.000001,-1);

    //cv::ximgproc::guidedFilter(InputArray guide, InputArray src, OutputArray dst, int radius, double eps, int dDepth=-1);

    waitKey(0);

}
