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
    Mat thermal_image = imread(thermal_path,CV_LOAD_IMAGE_GRAYSCALE);


    if (rgb_image.empty() || thermal_image.empty())
    {
        std::cout << "invalid file " << filename << std::endl;
        return;
    }



    std::cout << thermal_image.size() << std::endl;



    //imshow("test rgb", rgb_image);
    //imshow("test thermal", thermal_image);


    Mat output;
    Mat output_coloured;
    Mat output_eps_1_higher;


    //guided filter syntax
    //C++: void guidedFilter(InputArray guide, InputArray src, OutputArray dst, int radius, double eps, int dDepth=-1)

    //ximgproc::guidedFilter(rgb_image,thermal_image,output,20,1,-1);

    //imshow("test pan sharpen 1", output);


    //ximgproc::guidedFilter(rgb_image,thermal_image,output,20,1000,-1);

    //imshow("test pan sharpen 2", output);


    int rad_input = 200;
    int eps_input = 1;

    namedWindow("PS test");


    cvCreateTrackbar("radius","PS test",&rad_input,640);
    cvCreateTrackbar("epsilon","PS test",&eps_input,14);


    while(1)
    {
        if(eps_input==0)
        {
            eps_input = 1;
        }

        if(rad_input==0)
        {
            rad_input = 1;
        }


        ximgproc::guidedFilter(rgb_image,thermal_image,output,rad_input,1.*eps_input,-1);
        ximgproc::guidedFilter(rgb_image,thermal_image,output_eps_1_higher,rad_input,1.*eps_input+10,-1);

        output = 100*(output - output_eps_1_higher);

        if(abs(mean(output)[0])<=1)
        {
            std::cout << "you got problems" << std::endl;
        }

        applyColorMap(output, output_coloured, COLORMAP_JET );

        imshow("PS test", output_coloured);

        waitKey(66);

    }


    //std::cout << output << std::endl;


    //cv::ximgproc::guidedFilter(InputArray guide, InputArray src, OutputArray dst, int radius, double eps, int dDepth=-1);

    waitKey(0);

}
