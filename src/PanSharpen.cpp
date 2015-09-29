#include "include/PanSharpen.h"

using namespace cv;

PanSharpen::PanSharpen()
{
}

void PanSharpen::read_from_disk(std::string filename)
{
    Mat image = imread(filename);

    if (image.empty())
    {
        std::cout << "invalid file " << filename << std::endl;
        return;
    }

    imshow("test", image);
    waitKey(0);

}
