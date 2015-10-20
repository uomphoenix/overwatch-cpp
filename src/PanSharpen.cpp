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
    Mat visual_greyscale = imread(rgb_path,CV_LOAD_IMAGE_GRAYSCALE);


    if (rgb_image.empty() || thermal_image.empty())
    {
        std::cout << "invalid file " << filename << std::endl;
        return;
    }



    //std::cout << thermal_image.size() << std::endl;



    //imshow("test rgb", rgb_image);
    //imshow("test thermal", thermal_image);


    Mat final_output;
    Mat output_guided_filter;
    Mat output_coloured;
    Mat output_eps_1_higher;
    Mat visual_blurred;
    Mat unsharp_mask;
    Mat grayscale_blurred;
    Mat filter_plus_mak;
    Mat std_filtered_image;


    //guided filter syntax
    //C++: void guidedFilter(InputArray guide, InputArray src, OutputArray dst, int radius, double eps, int dDepth=-1)

    //ximgproc::guidedFilter(rgb_image,thermal_image,output,20,1,-1);

    //imshow("test pan sharpen 1", output);


    //ximgproc::guidedFilter(rgb_image,thermal_image,output,20,1000,-1);

    //imshow("test pan sharpen 2", output);


    int rad_input = 200;
    int eps_input = 1;
    int kernel_size = 1;
    int signmaX = 5;
    namedWindow("PS test");


    cvCreateTrackbar("radius","PS test",&rad_input,640);
    cvCreateTrackbar("epsilon","PS test",&eps_input,14);
    cvCreateTrackbar("kernal size (radius)","PS test",&kernel_size,100);
    cvCreateTrackbar("sigma X","PS test",&signmaX,14);

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


        ximgproc::guidedFilter(rgb_image,thermal_image,output_guided_filter,rad_input,1.*eps_input,-1);

        std_filtered_image = std_deviation_filter(visual_greyscale);

        filter_plus_mak = output_guided_filter + std_filtered_image;

        //ximgproc::guidedFilter(rgb_image,thermal_image,output_eps_1_higher,rad_input,1.*eps_input+10,-1);

        /*
        GaussianBlur(visual_greyscale, grayscale_blurred, Size(kernel_size*2+1,kernel_size*2+1), signmaX);
        unsharp_mask = visual_greyscale - grayscale_blurred;
        filter_plus_mak = output_guided_filter + unsharp_mask;
        */

        /*
        if(abs(mean(output)[0])<=1)
        {
            std::cout << "you got problems" << std::endl;
        }
        */

        applyColorMap(filter_plus_mak, final_output, COLORMAP_JET );

        //imshow("before unsharp mask is added", output_guided_filter);
        imshow("PS test", final_output);

        waitKey(66);

    }


    //std::cout << output << std::endl;


    //cv::ximgproc::guidedFilter(InputArray guide, InputArray src, OutputArray dst, int radius, double eps, int dDepth=-1);

    waitKey(0);

}


Mat PanSharpen::std_deviation_filter(Mat input)
{

    Mat input_as_double;
    Mat output_as_double;
    Mat output;
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

    output_as_double.convertTo(output,CV_8UC3);




    /*
    //Rect roi(1, 1, 5, 5);
    //Mat roi_1 = output(roi);
    //std::cout << roi_1 << std::endl;
    */
    return output;

}
