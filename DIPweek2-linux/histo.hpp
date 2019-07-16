#ifndef HISTO_HPP
#define HISTO_HPP

#include "globals.hpp"

// template<typename _Tx>
std::array<int, 256> histogram( cv::Mat& img );
std::array<int, 256> indexed_histogram( cv::Mat& img, cv::Mat index );
std::array<int, 1024> histogram_ex( cv::Mat& img );

// estimate global threshold
double global_threshold( cv::Mat Img );

// estimate otsu threshold
double otsu_threshold( cv::Mat Img );
double otsu_threshold_indexed( cv::Mat Img, cv::Mat Index );

#endif  // HISTO_HPP
