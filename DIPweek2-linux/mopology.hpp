#ifndef MOPOLOGY_HPP
#define MOPOLOGY_HPP

#include "globals.hpp"

cv::Mat rectSE( const int size );

// A (-) B
cv::Mat erosion( cv::Mat A, cv::Mat B );

// A (+) B
cv::Mat dilation( cv::Mat A, cv::Mat B );

// A () B
cv::Mat opening( cv::Mat A, cv::Mat B );

// A (//) B
cv::Mat closing( cv::Mat A, cv::Mat B );

#endif  // MOPOLOGY_HPP