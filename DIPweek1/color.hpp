#ifndef COLOR_HPP
#define COLOR_HPP

#include "globals.hpp"

#include "basic.hpp"

cv::Mat ColorTransform(
    cv::Mat img,
    std::function<cv::Vec3b(cv::Vec3b)> func
);

cv::Vec3b ColorIdentity(cv::Vec3b color);
cv::Vec3b BGR2HSI(cv::Vec3b color);
cv::Vec3b HSI2BGR(cv::Vec3b color);

#endif //COLOR_HPP
