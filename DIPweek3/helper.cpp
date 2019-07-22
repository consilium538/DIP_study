#include "helper.hpp"

cv::Vec3b HSI2BGR( cv::Vec3d color )
{
    double R, G, B;
    double rad_H;
    if ( color[0] < 256.0 / 3 )  // RG sector
    {
        rad_H = ( color[0] / 128.0 ) * M_PI;
        B = color[2] * ( 256.0 - color[1] ) / 256.0;
        R = color[2] *
            ( 256.0 +
              color[1] * std::cos( rad_H ) / std::cos( M_PI / 3.0 - rad_H ) ) /
            256.0;
        G = 3 * (double)color[2] - ( R + B );
    }
    else if ( color[0] < 256.0 * 2 / 3 )  // GB sector
    {
        rad_H = ( color[0] / 128.0 - 2.0 / 3.0 ) * M_PI;
        R = color[2] * ( 256.0 - color[1] ) / 256.0;
        G = color[2] *
            ( 256.0 +
              color[1] * std::cos( rad_H ) / std::cos( M_PI / 3.0 - rad_H ) ) /
            256.0;
        B = 3 * (double)color[2] - ( R + G );
    }
    else  // BR sector
    {
        rad_H = ( color[0] / 128.0 - 4.0 / 3.0 ) * M_PI;
        G = color[2] * ( 256.0 - color[1] ) / 256.0;
        B = color[2] *
            ( 256.0 +
              color[1] * std::cos( rad_H ) / std::cos( M_PI / 3.0 - rad_H ) ) /
            256.0;
        R = 3 * (double)color[2] - ( G + B );
    }
    return cv::Vec3b( cv::saturate_cast<uchar>(B),
                      cv::saturate_cast<uchar>(G),
                      cv::saturate_cast<uchar>(R) );
}

cv::Vec3b circularcolor(unsigned int a)
{
    return HSI2BGR(cv::Vec3b(a*7.0*256.0/11.0,128.0,128.0));
}