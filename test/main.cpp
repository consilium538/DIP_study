#define _USE_MATH_DEFINES

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "intensity.hpp"
#include "histo.hpp"
#include "color.hpp"

using namespace std;
using namespace cv;

int main( int argv, char** argc )
{
    const string inputPath =
        "/mnt/win/KDH/19ysummer/dip/DIP4E Book Images Global Edition/";
    const string arrowFile = "strawberries-RGB.tif";
    const string savepath = "./tmpImg/";

    Mat originalImg = cv::imread( inputPath + arrowFile );
    Mat TransformedImg, MiddleImg, AbcImg;
    Mat MiddleImgArr[3];

//    TransformedImg =
//        intensityTransform( originalImg, gammatransform( 1, 3 ) );
    MiddleImg = ColorTransform(originalImg,BGR2HSI);
    cv::split( MiddleImg, MiddleImgArr );
    TransformedImg = ColorTransform(MiddleImg, HSI2BGR);

    cv::namedWindow( "Original", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "Original", 20, 20 );
    cv::imshow( "Original", originalImg );

    cv::namedWindow( "H", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "H", 500, 20 );
    cv::imshow( "H", MiddleImgArr[0] );

    cv::namedWindow( "S", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "S", 20, 500 );
    cv::imshow( "S", MiddleImgArr[1] );

    cv::namedWindow( "I", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "I", 500, 500 );
    cv::imshow( "I", MiddleImgArr[2] );

    cv::namedWindow( "Transformed", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "Transformed", 1000, 20 );
    cv::imshow( "Transformed", TransformedImg );

    cv::waitKey( 0 );
    cv::destroyAllWindows();

    return 0;
}
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