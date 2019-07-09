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

using namespace std;
using namespace cv;

int main( int argv, char** argc )
{
    const string inputPath =
        "/mnt/win/KDH/19ysummer/dip/DIP4E Book Images Global Edition/";
    const string arrowFile = "spine.tif";
    const string savepath = "./tmpImg/";

    Mat originalImg = cv::imread( inputPath + arrowFile, cv::IMREAD_GRAYSCALE );
    Mat TransformedImg, MiddleImg, AbcImg;
    Mat MiddleImgArr[3];

    TransformedImg =
        intensityTransform( originalImg, gammatransform( 1, 0.4 ) );

    cv::namedWindow( "Original", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "Original", 20, 20 );
    cv::imshow( "Original", originalImg );

    cv::namedWindow( "Transformed", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "Transformed", 600, 20 );
    cv::imshow( "Transformed", TransformedImg );

    cv::waitKey( 3000 );
    cv::destroyAllWindows();

    return 0;
}

// fig 9.5   : circuitmask.tif
// fig 9.7   : text-broken.tif
// fig 9.11  : fingerprint-noisy.tif
// fig 9.16  : lincoln.tif
// fig 9.18  : balls-with-reflections.tif
// fig 9.20  : chickenXray.tif
// fig 9.33~ : text.tif

//    cv::imwrite( savepath + "moon-blurred.tif", TransformedImg );
