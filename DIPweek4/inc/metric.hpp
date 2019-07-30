#ifndef METRIC_HPP
#define METRIC_HPP

#include "globals.hpp"

double
mad_patch( cv::Mat ref,
           cv::Mat comp,
           double dx,
           double dy,
           std::vector<double>& args );
double
mse_patch( cv::Mat ref,
           cv::Mat comp,
           double dx,
           double dy,
           std::vector<double>& args );

#endif  // METRIC_HPP