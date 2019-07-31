#ifndef METRIC_HPP
#define METRIC_HPP

#include "globals.hpp"

double
mad_patch( const cv::Mat ref,
           const cv::Mat comp,
           const double dx,
           const double dy,
           const std::vector<double>& args = std::vector<double>() );

double
mse_patch( const cv::Mat ref,
           const cv::Mat comp,
           const double dx,
           const double dy,
           const std::vector<double>& args = std::vector<double>() );

double
mad_dist( const cv::Mat ref,
          const cv::Mat comp,
          const double dx,
          const double dy,
          const std::vector<double>& args );

#endif  // METRIC_HPP