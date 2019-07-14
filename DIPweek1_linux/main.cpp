#include "main.hpp"

int main( int argv, char** argc )
{
    const string inputPath = "./srcImg/";
    const string savepath = "./dstImg/";
    std::vector<std::tuple<cv::Mat, string>> ImgArr;

    catCPUID( std::cout );

    cv::Mat lens_orig = cv::imread(inputPath+"contact-lens.tif", cv::IMREAD_GRAYSCALE);
    ImgArr.push_back(std::make_tuple(lens_orig,"lens_orig"));

    cv::Mat lens_double;
    lens_orig.convertTo(lens_double,CV_64F);

    cv::Mat lens_sobel_x = conv2d(lens_double, sobel_filter(SobelOption::xdir), Padding::replicate);
    cv::Mat lens_sobel_y = conv2d(lens_double, sobel_filter(SobelOption::ydir), Padding::replicate);

    cv::Mat lens_sobel_x_norm = cv::abs(lens_sobel_x);
    cv::Mat lens_sobel_y_norm = cv::abs(lens_sobel_y);

    cv::Mat lens_sobel_x_int, lens_sobel_y_int;
    lens_sobel_x.convertTo(lens_sobel_x_int,CV_8U,0.25,128);
    ImgArr.push_back(std::make_tuple(lens_sobel_x_int,"lens_sobel_x_0.25"));
    lens_sobel_y.convertTo(lens_sobel_y_int,CV_8U,0.25,120);
    ImgArr.push_back(std::make_tuple(lens_sobel_y_int,"lens_sobel_y_0.25"));
    
    cv::Mat lens_grad = lens_sobel_x_norm + lens_sobel_y_norm;
    cv::Mat lens_grad_int;
    lens_grad.convertTo(lens_grad_int,CV_8U,0.25);
    ImgArr.push_back(std::make_tuple(lens_grad_int,"lens_grad_0.25"));

    //img_cat(ImgArr);
    img_save(ImgArr,savepath,".tif");

    return 0;
}

//    cv::Mat moon_orig = cv::imread(inputPath+"blurry-moon.tif", cv::IMREAD_GRAYSCALE);
//    ImgArr.push_back(std::make_tuple(moon_orig,"moon_orig"));
//
//    cv::Mat moon_double;
//    moon_orig.convertTo(moon_double,CV_64F);
//
//    cv::Mat moon_laplacian = conv2d(moon_double, laplacian_filter(LaplaceOption::four), Padding::replicate);
//    cv::Mat moon_lapl_norm;
//    moon_laplacian.convertTo(moon_lapl_norm,CV_8U,2.0,128.0);
//    ImgArr.push_back(std::make_tuple(moon_lapl_norm,"moon_lapl_norm"));
//
//    cv::Mat moon_shapren = moon_double + moon_laplacian;
//    moon_shapren.convertTo(moon_shapren,CV_8U);
//    ImgArr.push_back(std::make_tuple(moon_shapren,"moon_shapren"));

//    cv::Mat text_blurred_orig = cv::imread(inputPath+"text-dipxe-blurred.tif", cv::IMREAD_GRAYSCALE);
//    ImgArr.push_back(std::make_tuple(text_blurred_orig,"text_blurred_orig"));
//
//    cv::Mat text_blurred_double;
//    text_blurred_orig.convertTo(text_blurred_double,CV_64F);
//
//    cv::Mat text_blurred_gaussian = conv2d(text_blurred_double, gaussian_filter(31,5.0), Padding::replicate);
//    cv::Mat text_blurred_gaussian_int;
//    text_blurred_gaussian.convertTo(text_blurred_gaussian_int,CV_8U);
//    ImgArr.push_back(std::make_tuple(text_blurred_gaussian_int,"text_blurred_gaussian"));
//
//    cv::Mat text_blurred_sharpen_1 = text_blurred_double + 1.0 * ( text_blurred_double - text_blurred_gaussian );
//    cv::Mat text_blurred_sharpen_1_int;
//    text_blurred_sharpen_1.convertTo(text_blurred_sharpen_1_int,CV_8U);
//    ImgArr.push_back(std::make_tuple(text_blurred_sharpen_1_int,"text_blurred_sharpen_1"));
//
//    cv::Mat text_blurred_sharpen_5 = text_blurred_double + 5.0 * ( text_blurred_double - text_blurred_gaussian );
//    cv::Mat text_blurred_sharpen_5_int;
//    text_blurred_sharpen_5.convertTo(text_blurred_sharpen_5_int,CV_8U);
//    ImgArr.push_back(std::make_tuple(text_blurred_sharpen_5_int,"text_blurred_sharpen_5"));