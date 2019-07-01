#ifndef INTENSITY_HPP
#define INTENSITY_HPP

#include <opencv2/core/utility.hpp>
#include <cmath>

cv::Mat intensityLUT(
	std::function<unsigned char(unsigned char)> func
);

cv::Mat intensityTransform(
	cv::Mat img,
	std::function<unsigned char(unsigned char)> func
);

cv::Mat intensityTransform_iter(
    cv::Mat img,
    std::function<double(double)> func
);

unsigned char inverse(unsigned char);
std::function<unsigned char(unsigned char)> logtransform(double c);
std::function<unsigned char(unsigned char)> gammatransform(double c, double r);

#endif // INTENSITY_HPP
