#ifndef MAIN_HPP
#define MAIN_HPP

#define _USE_MATH_DEFINES

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/time.h>

#include <cpuid.h>
#include <chrono>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>

#include "globals.hpp"

using namespace std;
using namespace cv;

const bool isLogFile = true;

std::string CPUID_string()
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

    return fmt::format( "CPU Type: {}", CPUBrandString );
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
