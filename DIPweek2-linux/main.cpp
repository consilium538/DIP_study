#define _USE_MATH_DEFINES

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "mopology.hpp"

using namespace std;
using namespace cv;

int main( int argv, char** argc )
{
    const string inputPath =
        "/mnt/win/KDH/19ysummer/dip/DIP4E Book Images Global Edition/";
    const string arrowFile = "fingerprint-noisy.tif";
    const string savepath = "./tmpImg/";

    Mat originalImg = cv::imread( inputPath + arrowFile, cv::IMREAD_GRAYSCALE );

    if ( originalImg.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto se = rectSE( 3 );

    //    auto TransformedImg = erosion(originalImg, rectSE(91));
    //    auto TransformedImg = dilation(originalImg, rectSE(3));

    auto eroded = erosion( originalImg, se );
    auto open = opening( originalImg, se );
    auto open_close = closing( open, se );

    //    cv::namedWindow( "Original", cv::WINDOW_AUTOSIZE );
    //    cv::moveWindow( "Original", 20, 20 );
    //    cv::imshow( "Original", originalImg );

    //    cv::namedWindow( "Erosion(91)", cv::WINDOW_AUTOSIZE );
    //    cv::moveWindow( "Erosion(91)", 500, 20 );
    //    cv::imshow( "Erosion(91)", TransformedImg );
    //    cv::namedWindow( "Dilation", cv::WINDOW_AUTOSIZE );
    //    cv::moveWindow( "Dilation", 500, 20 );
    //    cv::imshow( "Dilation", TransformedImg );

    //    cv::waitKey( 0 );
    //    cv::destroyAllWindows();

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
