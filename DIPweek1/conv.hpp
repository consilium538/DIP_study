#ifndef CONV_HPP
#define CONV_HPP

#include "globals.hpp"

enum class Padding
{
    noselected,
    zero,
    replicate,
    mirror
};
enum class LaplaceOption
{
    noselected,
    four,
    eight
};
enum class SobelOption
{
    noselected,
    xdir,
    ydir
};

// convolution methode
cv::Mat conv2d( cv::Mat large, cv::Mat small, Padding method );

// kernel generation functions
cv::Mat box_filter( int size );
cv::Mat gaussian_filter( int size, double deviation );
cv::Mat laplacian_filter( LaplaceOption way );
cv::Mat sobel_filter( SobelOption dir );

// specific algorithme
cv::Mat adaptive_median( cv::Mat orig, int maxWindowSize );

#endif  // CONV_HPP
