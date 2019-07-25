#ifndef CCL_HPP
#define CCL_HPP

#include "globals.hpp"

std::tuple<cv::Mat, int> grassfire_4( cv::Mat img );
std::tuple<cv::Mat, int> grassfire_8( cv::Mat img );
std::tuple<cv::Mat, int> eq_label_set_8( cv::Mat img );
std::tuple<cv::Mat, int> eq_label_set_4( cv::Mat img );
std::tuple<cv::Mat, int> contour_tarck_8( cv::Mat img );
std::tuple<cv::Mat, int> contour_tarck_4( cv::Mat img );

#endif  // CCL_HPP
