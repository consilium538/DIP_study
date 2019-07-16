#ifndef INTENSITY_HPP
#define INTENSITY_HPP

#include "globals.hpp"

cv::Mat intensityLUT( std::function<unsigned char( unsigned char )> func );

cv::Mat intensityTransform(
    cv::Mat img,
    std::function<unsigned char( unsigned char )> func );

cv::Mat intensityTransform_iter( cv::Mat img,
                                 std::function<double( double )> func );

unsigned char inverse( unsigned char );
std::function<unsigned char( unsigned char )> logtransform( double c );
std::function<unsigned char( unsigned char )> gammatransform( double c,
                                                              double r );
std::function<unsigned char( unsigned char )> thresholding(
    unsigned char threshold );
std::function<unsigned char( unsigned char )> levelslicing( int lower,
                                                            int upper,
                                                            int level );
std::function<unsigned char( unsigned char )> bitslicing(
    unsigned char bitlevel );
cv::Mat testGaussian( cv::Mat Img, double mean, double stddev );

#endif  // INTENSITY_HPP
