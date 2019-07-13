#define _USE_MATH_DEFINES

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "color.hpp"
#include "histo.hpp"
#include "intensity.hpp"

using namespace std;
using namespace cv;

void img_cat( std::vector<std::tuple<cv::Mat, string>> ImgArr )
{
    int a = 20;
    for ( auto it : ImgArr )
    {
        cv::namedWindow( std::get<1>( it ), cv::WINDOW_AUTOSIZE );
        cv::moveWindow( std::get<1>( it ), a, 20 );
        cv::imshow( std::get<1>( it ), std::get<0>( it ) );
        a += 50;
    }

    if( ImgArr.size() != 0 )
    {
        cv::waitKey( 0 );
        cv::destroyAllWindows();
    }

    return;
}

void img_save( std::vector<std::tuple<cv::Mat, string>> ImgArr,
               const string savepath, const string postfix,
               const std::vector<int>& params = std::vector< int >() )
{
    for ( auto it : ImgArr )
    {
        cv::imwrite( savepath + std::get<1>( it ) + postfix, std::get<0>( it ),
                     params );
    }
}

int main( int argv, char** argc )
{
    const string inputPath = "./srcImg/";
    const string arrowFile = "RGB-color-cube.tif";
    const string savepath = "./dstImg/";

    std::vector<std::tuple<cv::Mat, string>> ImgArr;

    Mat originalImg = cv::imread( inputPath + arrowFile );
    if ( originalImg.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    cv::Mat TransformedImg, MiddleImg, AbcImg;
    cv::Mat MiddleImgArr[3];

    img_cat( ImgArr );
    img_save( ImgArr, savepath, ".tif" );

    return 0;
}


////////////////// color_cube ////////////////////
//    MiddleImg = ColorTransform( originalImg, BGR2HSI );
//    ImgArr.push_back( std::make_tuple( originalImg, "color_cube_orig" ) );
//
//    cv::split( MiddleImg, MiddleImgArr );
//
//    ImgArr.push_back( std::make_tuple( MiddleImgArr[0], "color_cube_H" ) );
//    ImgArr.push_back( std::make_tuple( MiddleImgArr[1], "color_cube_S" ) );
//    ImgArr.push_back( std::make_tuple( MiddleImgArr[2], "color_cube_I" ) );
//
//    TransformedImg = ColorTransform( MiddleImg, HSI2BGR );
//    ImgArr.push_back( std::make_tuple( TransformedImg, "color_cube_reconst" ) );
//////////////////////////////////////////////////


//    cout << "Histogram calculation\n";
//    cout << "ColorChannel count : " << originalImg.channels()
//         << "\n" << endl;
//
//    auto histo = histogram(originalImg);
//    cout << "Original Histogram\n";
//    for(int i = 0; i < 255; i++)
//    {
//        cout << histo[i] << ", ";
//    }
//    cout << histo[255] << endl;
//
//    auto histo_trans = histogram(TransformedImg);
//    cout << "Transfromed Histogram\n";
//    for(int i = 0; i < 255; i++)
//    {
//        cout << histo_trans[i] << ", ";
//    }
//    cout << histo_trans[255] << endl;