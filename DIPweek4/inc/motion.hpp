#ifndef MOTION_HPP
#define MOTION_HPP

#include "globals.hpp"

using obj_f = std::function<double( const cv::Mat,
                                    const cv::Mat,
                                    const double,
                                    const double,
                                    const std::vector<double>& )>;

std::vector<std::tuple<int, int, int, int, int, int, double>>
ebma( cv::Mat ancher_img,
      cv::Mat tracked_img,
      const int block_size,
      const int search_range,
      obj_f objective,
      const std::vector<double>& obj_args );

#endif  // MOTION_HPP