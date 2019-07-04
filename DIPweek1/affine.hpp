#ifndef AFFINE_HPP
#define AFFINE_HPP

#include "globals.hpp"

enum class InterpolationMethod
{
    noselected,
    nearest,
    bilinear
};

cv::Mat AffineMat( double a11,
                   double a12,
                   double a13,
                   double a21,
                   double a22,
                   double a23 );

cv::Mat rotateMat( double theta );
cv::Mat translationMat( double tx, double ty );

std::tuple<double, double> AffineTransform( cv::Mat matrix,
                                            double x,
                                            double y );

cv::Mat GeometricTransform( cv::Mat sourceImg,
                            cv::Mat affine,
                            InterpolationMethod mode );

cv::Mat ResizeTransform( cv::Mat sourceImg,
                         double scaleX,
                         double scaleY,
                         InterpolationMethod mode );

#endif  // AFFINE_HPP