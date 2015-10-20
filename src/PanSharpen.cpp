#include <assert.h>

#include "include/PanSharpen.h"

using namespace cv;

PanSharpen::PanSharpen(int radius, double epsilon)
{
    assert(epsilon > 0);
    assert(radius > 0);

    this->radius = radius;
    this->epsilon = epsilon;
}

void PanSharpen::read_from_disk(std::string filename)
{
    namedWindow("PS test");

    std::string rgb_path = "/home/dcandy/Downloads/" + filename + "_rgb.jpg";
    std::string thermal_path = "/home/dcandy/Downloads/" + filename + "_thermal_smoothed.jpg";


    Mat rgb_image = imread(rgb_path);
    Mat thermal_image = imread(thermal_path,CV_LOAD_IMAGE_GRAYSCALE);

    if (rgb_image.empty() || thermal_image.empty())
    {
        std::cout << "invalid file " << filename << std::endl;
        return;
    }

    Mat sharpened;
    sharpen(thermal_image, rgb_image, sharpened);

    imshow("PS Test", sharpened);

    waitKey(0);

}

void PanSharpen::sharpen(Mat& thermal, Mat& visual, Mat &output)
{
    Mat output_guided_filter;
    Mat filter_plus_mak;
    Mat std_filtered_image;

    ximgproc::guidedFilter(visual,thermal, output_guided_filter, radius,
                           epsilon, -1);

    Mat visual_grayscale;
    cvtColor(visual, visual_grayscale, CV_RGB2GRAY);

    std_deviation_filter(visual_grayscale, std_filtered_image);

    filter_plus_mak = output_guided_filter + std_filtered_image;

    applyColorMap(filter_plus_mak, output, COLORMAP_JET);
}


void PanSharpen::std_deviation_filter(Mat& input, Mat& output)
{
    Mat input_as_double;
    Mat output_as_double;
    Mat c1;
    Mat c2;
    Mat no_negatives;

    Mat mean_blur;

    Mat kernel = Mat::ones(3, 3, CV_64F);  //32F might need to change
    int n = 9;
    double n1 = n-1;

    input.convertTo(input_as_double,CV_64F);



    Mat input_squared = input_as_double.mul(input_as_double);
    Mat kernel_on_n1 = kernel.mul(1./n1);


    filter2D(input_squared,c1,-1, kernel_on_n1);
    filter2D(input_as_double,mean_blur,-1, kernel);

    Mat mean_blur_squared = mean_blur.mul(mean_blur);

    c2 = mean_blur_squared.mul(1./(n*n1));

    Mat difference = c1 - c2;

    cv::sqrt(difference,output_as_double);

    output_as_double.convertTo(output, CV_8UC3);

    /*
    //Rect roi(1, 1, 5, 5);
    //Mat roi_1 = output(roi);
    //std::cout << roi_1 << std::endl;
    */
}
