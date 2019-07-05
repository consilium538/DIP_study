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

template <typename _Tx>
cv::Mat GeometricTransform( cv::Mat sourceImg,
                            cv::Mat affine,
                            InterpolationMethod mode );

template <typename _Tx>
cv::Mat ResizeTransform( cv::Mat sourceImg,
                         double scaleX,
                         double scaleY,
                         InterpolationMethod mode );

//template <typename _Tx>
//void affineLoop( cv::Mat sourceImg,
//                 cv::Mat& TransformedImg,
//                 cv::Mat inverseAffine,
//                 InterpolationMethod mode );

template <typename _Tx>
_Tx zeroPadded( cv::Mat& Img, int x, int y, int xb, int yb );

inline bool isBounderyIn( int x, int y, int xb, int yb );

// template function body

template <typename _Tx>
void affineLoop( cv::Mat_<_Tx> sourceImg,
                 cv::Mat& TransformedImg,
                 cv::Mat inverseAffine,
                 InterpolationMethod mode );


template <typename _Tx>
cv::Mat ResizeTransform( cv::Mat sourceImg,
                         double scaleX,
                         double scaleY,
                         InterpolationMethod mode )
{
    const int nRows = (int)( scaleX * sourceImg.rows );
    const int nCols = (int)( scaleY * sourceImg.cols );
    double inversescaleX = ( (double)sourceImg.rows - 1.0 ) /
                           ( scaleX * (double)sourceImg.rows - 1.0 );
    double inversescaleY = ( (double)sourceImg.cols - 1.0 ) /
                           ( scaleY * (double)sourceImg.cols - 1.0 );

    cv::Mat TransformedImg = cv::Mat_<_Tx>( nRows, nCols );
    cv::Mat inverseAffine = ( cv::Mat_<double>( 3, 3 ) << inversescaleX, 0, 0,
                              0, inversescaleY, 0, 0, 0, 1 );

    affineLoop<_Tx>( sourceImg, TransformedImg, inverseAffine, mode );

    return TransformedImg.clone();
}

template <typename _Tx>
cv::Mat GeometricTransform( cv::Mat sourceImg,
                            cv::Mat affine,
                            InterpolationMethod mode )
{
    cv::Mat TransformedImg = cv::Mat_<_Tx>( sourceImg.size() );
    cv::Mat inverseAffine = affine.inv();

    affineLoop<_Tx>( sourceImg, TransformedImg, inverseAffine, mode );

    return TransformedImg;
}

bool isBounderyIn( int x, int y, int xb, int yb )
{
    if ( x < 0 )
        return false;
    if ( x >= xb )
        return false;
    if ( y < 0 )
        return false;
    if ( y >= yb )
        return false;
    else
        return true;
}

template <typename _Tx>
_Tx zeroPadded( cv::Mat& Img, int x, int y, int xb, int yb )
{
    if ( isBounderyIn( x, y, xb, yb ) )
        return Img.at<_Tx>( x, y );
    else
        return 0;
}

template <typename _Tx>
_Tx bilinear(cv::Mat& Img, int x, int y, int xb, int yb, double dx, double dy)
{
    
}

#endif  // AFFINE_HPP
