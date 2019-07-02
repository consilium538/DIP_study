#ifndef AFFINE_HPP
#define AFFINE_HPP

#include <opencv2/core/utility.hpp>
#include <cmath>

cv::Mat AffineMat(
    double a11, double a12, double a13,
    double a21, double a22, double a23
);

std::tuple<double, double> AffineTransform(
    cv::Mat matrix, double x, double y
);

cv::Mat GeometricTransform(
    cv::Mat sourceImg,
    cv::Mat affine
);

cv::Mat ResizeTransform(
    cv::Mat sourceImg,
    double scaleX,
    double scaleY,
    int mode
);

#endif // AFFINE_HPP