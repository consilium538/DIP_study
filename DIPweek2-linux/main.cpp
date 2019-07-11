#define _USE_MATH_DEFINES

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "histo.hpp"
#include "intensity.hpp"
#include "main.hpp"
#include "mopology.hpp"

using namespace std;
using namespace cv;

int main( int argv, char** argc )
{
    const string inputPath = "./srcImg/";
    const string savepath = "./tmpImg/";
    std::vector<std::tuple<cv::Mat, string>> ImgArr;

////////////////////////////////////////
#ifndef SKIP_EROSION

    auto circuitmask_orig =
        cv::imread( inputPath + "circuitmask.tif", cv::IMREAD_GRAYSCALE );
    cv::resize( circuitmask_orig, circuitmask_orig, Size(), 0.5, 0.5,
                INTER_NEAREST );
    ImgArr.push_back( std::make_tuple( circuitmask_orig, "circuitmask_orig" ) );

    if ( circuitmask_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto circuitmask_11 = erosion( circuitmask_orig, rectSE( 11 ) );
    ImgArr.push_back( std::make_tuple( circuitmask_11, "circuitmask_11" ) );

    auto circuitmask_15 = erosion( circuitmask_orig, rectSE( 15 ) );
    ImgArr.push_back( std::make_tuple( circuitmask_15, "circuitmask_15" ) );

    auto circuitmask_45 = erosion( circuitmask_orig, rectSE( 45 ) );
    ImgArr.push_back( std::make_tuple( circuitmask_45, "circuitmask_45" ) );

#endif  // SKIP_EROSION
////////////////////////////////////////
#ifndef SKIP_DILATION  // text-broken.tif

    auto text_broken_orig =
        cv::imread( inputPath + "text-broken.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( text_broken_orig, "text_broken_orig" ) );

    if ( text_broken_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto text_broken_dil = dilation( text_broken_orig, rectSE( 3 ) );
    ImgArr.push_back( std::make_tuple( text_broken_dil, "text_broken_dil" ) );

#endif  // SKIP_DILATION
////////////////////////////////////////
#ifndef SKIP_OPENING

    auto finger_orig =
        cv::imread( inputPath + "fingerprint-noisy.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( finger_orig, "finger_orig" ) );

    if ( finger_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto se = rectSE( 3 );

    auto finger_eroded = erosion( finger_orig, se );
    ImgArr.push_back( std::make_tuple( finger_eroded, "finger_eroded" ) );

    auto finger_open = opening( finger_orig, se );
    ImgArr.push_back( std::make_tuple( finger_open, "finger_open" ) );

    auto finger_open_close = closing( finger_open, se );
    ImgArr.push_back(
        std::make_tuple( finger_open_close, "finger_open_close" ) );

    ///    for(auto it:ImgArr)
    ///    {
    ///        std::get<0>(it);
    ///    }

#endif  // SKIP_OPENING
////////////////////////////////////////
#ifndef SKIP_BOUNDARY

    auto lincoln_orig =
        cv::imread( inputPath + "lincoln.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( lincoln_orig, "lincoln_orig" ) );

    if ( lincoln_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto lincoln_eros = erosion( lincoln_orig, rectSE( 3 ) );
    auto lincoln_bound = cv::Mat_<uchar>( lincoln_orig.size(),
                                          ball_lincoln_origorig.channels() );
    lincoln_bound.forEach( [&]( Pixel& p, const int* i ) {
        p = cv::saturate_cast<uchar>( lincoln_orig.at<uchar>( i[0], i[1] ) -
                                      lincoln_eros.at<uchar>( i[0], i[1] ) );
    } );
    ImgArr.push_back( std::make_tuple( lincoln_bound, "lincoln_bound" ) );

#endif  // SKIP_BOUNDARY
////////////////////////////////////////
#ifndef SKIP_HOLE  // balls-with-reflections.tif
                   // crossSE
    auto ball_orig = cv::imread( inputPath + "balls-with-reflections.tif",
                                 cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( ball_orig, "ball_orig" ) );

    if ( ball_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    std::vector<std::tuple<int, int>> hole_init = {
        {60, 50},   {180, 45},  {363, 39},  {93, 159},  {178, 203},
        {262, 148}, {414, 233}, {461, 142}, {104, 299}, {236, 308},
        {386, 371}, {489, 357}, {56, 451},  {235, 460}, {388, 371},
        {489, 358}, {58, 451},  {234, 459}, {505, 490}, {399, 471}};

    auto ball_hole = cv::Mat( ball_orig.size(), CV_8UC1, cv::Scalar( 0 ) );

    for ( auto it : hole_init )
        ball_hole.at<uchar>( std::get<1>( it ), std::get<0>( it ) ) = 255;

    cv::Mat ball_dilated;
    auto ball_filled = cv::Mat( ball_orig.size(), CV_8UC1, cv::Scalar( 0 ) );
    bool noChange = true;

    for ( int i = 0; i < 50; i++ )
    {
        ball_dilated = dilation( ball_hole, crossSE() );

        ball_filled.forEach<uchar>( [&]( Pixel& p, const int* i ) {
            p = ball_orig.at<uchar>( i[0], i[1] ) == 0 &&
                        ball_dilated.at<uchar>( i[0], i[1] ) == 255
                    ? 255
                    : 0;
            if ( p != ball_hole.at<uchar>( i[0], i[1] ) )
                noChange = false;
        } );

        if ( noChange == true )
            break;

        noChange = true;
        ball_hole.release();
        ball_hole = ball_filled.clone();
    }

    auto ball_completed = cv::Mat( ball_orig.size(), CV_8UC1, cv::Scalar( 0 ) );
    ball_completed.forEach<uchar>( [&]( Pixel& p, const int* i ) {
        p = cv::saturate_cast<uchar>( ball_orig.at<uchar>( i[0], i[1] ) +
                                      ball_hole.at<uchar>( i[0], i[1] ) );
    } );
    ImgArr.push_back( std::make_tuple( ball_completed, "ball_completed" ) );

#endif  // SKIP_HOLE
////////////////////////////////////////
#ifndef SKIP_CONNECTED  // chickenXray.tif

    auto chicken_orig = cv::imread( inputPath + "chickenXray-thresholded.tif",
                                    cv::IMREAD_GRAYSCALE );
    cv::resize( chicken_orig, chicken_orig, Size(), 0.5, 0.5, INTER_NEAREST );
    ImgArr.push_back( std::make_tuple( chicken_orig, "chicken_orig" ) );

    if ( chicken_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    std::vector<std::tuple<int, int>> cc_init = {{0, 0}};

#endif  // SKIP_CONNECTED
////////////////////////////////////////
#ifndef SKIP_RECONSTRUCTION  // text.tif
                             // 25*1

    auto text_orig = cv::imread( inputPath + "text.tif", cv::IMREAD_GRAYSCALE );
    if ( text_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    cv::resize( text_orig, text_orig, Size(), 0.5, 0.5, INTER_NEAREST );
    ImgArr.push_back( std::make_tuple( text_orig, "text_orig" ) );

#ifndef SKIP_RECON_OPEN
    auto text_eros = erosion( text_orig, rectSE( 25, 1 ) );
    auto text_opening = opening( text_orig, rectSE( 25, 1 ) );
    auto text_reconst =
        geodesic_reconst_D( text_opening, text_orig, rectSE( 3 ) );
#endif  // SKIP_RECON_OPEN

#ifndef SKIP_RECON_FILL
    cv::Mat text_invert = 255 - text_orig;
    auto text_filled = auto_hole( text_orig );

//    cv::namedWindow("text_orig",WINDOW_AUTOSIZE);
//    cv::moveWindow("text_orig", 20, 20);
//    cv::imshow("test_orig",text_orig);
//
//    cv::namedWindow("text_filled",WINDOW_AUTOSIZE);
//    cv::moveWindow("text_filled", 20, 20);
//    cv::imshow("text_filled",text_filled);
//
//    cv::waitKey(0);
//    cv::destroyAllWindows();
#endif  // SKIP_RECON_FILL

#ifndef SKIP_RECON_BORDER
    auto text_border_clean = border_clean( text_orig );
    cv::Mat text_border = text_orig - text_border_clean;
#endif  // SKIP_RECON_BORD

#endif  // SKIP_RECONSTRUCTION
////////////////////////////////////////
#ifndef SKIP_GLOBAL  // fingerprint.tif
    auto finger_orig =
        cv::imread( inputPath + "fingerprint.tif", cv::IMREAD_GRAYSCALE );
    if ( finger_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back( std::make_tuple( finger_orig, "finger_orig" ) );
    auto finger_global_th = global_threshold( finger_orig );

    auto finger_global_seg =
        intensityTransform( finger_orig, thresholding( finger_global_th ) );

#endif  // SKIP_GLOBAL
////////////////////////////////////////
#ifndef SKIP_OTSU  // polymercell.tif

#endif  // SKIP_OTSU
////////////////////////////////////////
#ifndef SKIP_EDGE_GRAD  //

#endif  // SKIP_EDGE_GRAD
////////////////////////////////////////
#ifndef SKIP_LAPLACE_GRAD  //

#endif  // SKIP_LAPLACE_GRAD
////////////////////////////////////////
#ifndef SKIP_MULTIPLE_GRAD  //

#endif  // SKIP_MULTIPLE_GRAD
////////////////////////////////////////
#ifndef SKIP_VARIABLE  //

#endif  // SKIP_VARIABLE
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
