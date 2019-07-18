#ifndef MAIN_HPP
#define MAIN_HPP

#define _USE_MATH_DEFINES

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <cpuid.h>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "conv.hpp"
#include "histo.hpp"
#include "intensity.hpp"
#include "mopology.hpp"

using namespace std;
using namespace cv;

// #define RUN_ALL

#ifndef RUN_ALL
#define SKIP_EROSION
#define SKIP_DILATION
#define SKIP_OPENING
#define SKIP_BOUNDARY
#define SKIP_HOLE
#define SKIP_CONNECTED
#define SKIP_RECONSTRUCTION
//
#define SKIP_RECON_OPEN
#define SKIP_RECON_FILL
#define SKIP_RECON_BORDER
//
#define SKIP_GLOBAL
#define SKIP_OTSU
#define SKIP_SMOOTH_GLOBAL
#define SKIP_EDGE_GRAD
#define SKIP_EDGE_LAPLACE
#define SKIP_MULTIPLE_TH
// #define SKIP_VARIABLE_IMG_LOCAL
#define SKIP_VARIABLE_MOVING_AVG
#endif  // RUN_ALL

const bool isLogFile = false;

void catCPUID( std::ostream& out )
{
    char CPUBrandString[0x40];
    unsigned int CPUInfo[4] = {0, 0, 0, 0};

    __cpuid( 0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3] );
    unsigned int nExIds = CPUInfo[0];

    memset( CPUBrandString, 0, sizeof( CPUBrandString ) );

    for ( unsigned int i = 0x80000000; i <= nExIds; ++i )
    {
        __cpuid( i, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3] );

        if ( i == 0x80000002 )
            memcpy( CPUBrandString, CPUInfo, sizeof( CPUInfo ) );
        else if ( i == 0x80000003 )
            memcpy( CPUBrandString + 16, CPUInfo, sizeof( CPUInfo ) );
        else if ( i == 0x80000004 )
            memcpy( CPUBrandString + 32, CPUInfo, sizeof( CPUInfo ) );
    }

    out << "CPU Type: " << CPUBrandString << std::endl;
}

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

    if ( ImgArr.size() != 0 )
    {
        cv::waitKey( 0 );
        cv::destroyAllWindows();
    }

    return;
}

void img_save( std::vector<std::tuple<cv::Mat, string>> ImgArr,
               const string savepath,
               const string postfix,
               const std::vector<int>& params = std::vector<int>() )
{
    for ( auto it : ImgArr )
    {
        cv::imwrite( savepath + std::get<1>( it ) + postfix, std::get<0>( it ),
                     params );
    }
}

#endif  // MAIN_HPP
