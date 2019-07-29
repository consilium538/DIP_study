#ifndef METRIC_HPP
#define METRIC_HPP

#include "globals.hpp"

double mad_patch( cv::Mat ref, cv::Mat comp );
double mse_patch( cv::Mat ref, cv::Mat comp );

#endif // METRIC_HPP