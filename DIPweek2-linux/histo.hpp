#ifndef HISTO_HPP
#define HISTO_HPP

#include "globals.hpp"

// template<typename _Tx>
std::array<int, 256> histogram( cv::Mat& img );

// estimate global threshold
double global_threshold( cv::Mat Img );

#endif  // HISTO_HPP
