#define _USE_MATH_DEFINES

// #define RUN_ALL

#ifndef RUN_ALL
    #define SKIP_EROSION
    #define SKIP_DILATION
    #define SKIP_OPENING
    #define SKIP_BOUNDARY
    #define SKIP_HOLE
    #define SKIP_CONNECTED
    #define SKIP_RECONSTRUCTION
    #define SKIP_GLOBAL
    #define SKIP_OTSU
    #define SKIP_EDGE_GRAD
    #define SKIP_LAPLACE_GRAD
    #define SKIP_MULTIPLE_GRAD
    #define SKIP_VARIABLE
#endif // RUN_ALL

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

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
        "./srcImg/";
    const string savepath = "./tmpImg/";
    std::vector<std::tuple<cv::Mat,string>> ImgArr;

////////////////////////////////////////
#ifndef SKIP_EROSION

    auto circuitmask_orig = cv::imread( inputPath + "circuitmask.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back(std::make_tuple(circuitmask_orig,"circuitmask_orig"));

    cv::resize(circuitmask_orig, circuitmask_orig, Size(), 0.5, 0.5, INTER_NEAREST);
    if ( circuitmask_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto circuitmask_11 = erosion( circuitmask_orig, rectSE( 11 ) );
    ImgArr.push_back(std::make_tuple(circuitmask_11,"circuitmask_11"));

    auto circuitmask_15 = erosion( circuitmask_orig, rectSE( 15 ) );
    ImgArr.push_back(std::make_tuple(circuitmask_15,"circuitmask_15"));

    auto circuitmask_45 = erosion( circuitmask_orig, rectSE( 45 ) );
    ImgArr.push_back(std::make_tuple(circuitmask_45,"circuitmask_45"));

#endif // SKIP_EROSION
////////////////////////////////////////
#ifndef SKIP_DILATION // text-broken.tif

    auto text_broken_orig = cv::imread( inputPath + "text-broken.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back(std::make_tuple(text_broken_orig,"text_broken_orig"));

    if ( text_broken_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto text_broken_dil = dilation( text_broken_orig, rectSE( 3 ) );
    ImgArr.push_back(std::make_tuple(text_broken_dil,"text_broken_dil"));

#endif // SKIP_DILATION
////////////////////////////////////////
#ifndef SKIP_OPENING

    auto finger_orig = cv::imread( inputPath + "fingerprint-noisy.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back(std::make_tuple(finger_orig,"finger_orig"));

    if ( finger_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto se = rectSE( 3 );

    auto finger_eroded = erosion( finger_orig, se );
    ImgArr.push_back(std::make_tuple(finger_eroded,"finger_eroded"));

    auto finger_open = opening( finger_orig, se );
    ImgArr.push_back(std::make_tuple(finger_open,"finger_open"));

    auto finger_open_close = closing( finger_open, se );
    ImgArr.push_back(std::make_tuple(finger_open_close,"finger_open_close"));

///    for(auto it:ImgArr)
///    {
///        std::get<0>(it);
///    }

#endif // SKIP_OPENING
////////////////////////////////////////
#ifndef SKIP_BOUNDARY



#endif // SKIP_BOUNDARY
////////////////////////////////////////
#ifndef SKIP_HOLE



#endif // SKIP_HOLE
////////////////////////////////////////
#ifndef SKIP_CONNECTED



#endif // SKIP_CONNECTED
////////////////////////////////////////
#ifndef SKIP_RECONSTRUCTION



#endif // SKIP_RECONSTRUCTION
////////////////////////////////////////
#ifndef SKIP_GLOBAL



#endif // SKIP_GLOBAL
////////////////////////////////////////
#ifndef SKIP_OTSU



#endif // SKIP_OTSU
////////////////////////////////////////
#ifndef SKIP_EDGE_GRAD



#endif // SKIP_EDGE_GRAD
////////////////////////////////////////
#ifndef SKIP_LAPLACE_GRAD



#endif // SKIP_LAPLACE_GRAD
////////////////////////////////////////
#ifndef SKIP_MULTIPLE_GRAD



#endif // SKIP_MULTIPLE_GRAD
////////////////////////////////////////
#ifndef SKIP_VARIABLE



#endif // SKIP_VARIABLE
////////////////////////////////////////

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
