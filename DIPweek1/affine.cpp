#include "affine.hpp"

cv::Mat AffineMat( double a11,
                   double a12,
                   double a13,
                   double a21,
                   double a22,
                   double a23 )
{
    double coeff[] = {a11, a12, a13, a21, a22, a23, 0.0, 0.0, 1.0};
    cv::Mat affine = cv::Mat( 3, 3, CV_64F, coeff );
    return affine.clone();
}

cv::Mat rotateMat( double theta )
{
    return ( cv::Mat_<double>( 3, 3 ) << std::cos( theta ), -std::sin( theta ),
             0, std::sin( theta ), std::cos( theta ), 0, 0, 0, 1 );
}

cv::Mat translationMat( double tx, double ty )
{
    return ( cv::Mat_<double>( 3, 3 ) << 1, 0, tx, 0, 1, ty, 0, 0, 1 );
}

std::tuple<double, double> AffineTransform( cv::Mat matrix, double x, double y )
{
    double* ptr = matrix.ptr<double>();
    return std::make_tuple( x * ptr[0] + y * ptr[1] + ptr[2],
                            x * ptr[3] + y * ptr[4] + ptr[5] );
}

template <>
void affineLoop<cv::Vec3b>( cv::Mat_<cv::Vec3b> sourceImg,
                            cv::Mat& TransformedImg,
                            cv::Mat inverseAffine,
                            InterpolationMethod mode )
{
    const int nRows = sourceImg.rows;
    const int nCols = sourceImg.cols;

    double k, q;
    int ik, iq;
    double dk, dq;

    for ( int i = 0; i < TransformedImg.rows; i++ )
    {
        for ( int j = 0; j < TransformedImg.cols; j++ )
        {
            std::tie( k, q ) = AffineTransform( inverseAffine, i, j );
            ik = mode == InterpolationMethod::nearest ? (int)std::round( k )
                                                      : (int)std::floor( k );
            dk = k - ik;
            iq = mode == InterpolationMethod::nearest ? (int)std::round( q )
                                                      : (int)std::floor( q );
            dq = q - iq;

            switch ( mode )
            {
                case InterpolationMethod::nearest:  // nearleast neighberhood
                    TransformedImg.at<cv::Vec3b>( i, j ) =
                        zeroPadded<cv::Vec3b>( sourceImg, ik, iq, nRows,
                                               nCols );
                    break;
                case InterpolationMethod::bilinear:
                    TransformedImg.at<cv::Vec3b>( i, j ) =
                        ( ( dk * dq ) *
                              cv::Vec3d( zeroPadded<cv::Vec3b>(
                                  sourceImg, ik + 1, iq + 1, nRows, nCols ) ) +
                          ( ( 1 - dk ) * dq ) *
                              cv::Vec3d( zeroPadded<cv::Vec3b>(
                                  sourceImg, ik, iq + 1, nRows, nCols ) ) +
                          ( dk * ( 1 - dq ) ) *
                              cv::Vec3d( zeroPadded<cv::Vec3b>(
                                  sourceImg, ik + 1, iq, nRows, nCols ) ) +
                          ( ( 1 - dk ) * ( 1 - dq ) ) *
                              cv::Vec3d( zeroPadded<cv::Vec3b>(
                                  sourceImg, ik, iq, nRows, nCols ) ) );
                    break;
                default:
                    break;
            }
        }
    }
}

template <>
void affineLoop<uchar>( cv::Mat_<uchar> sourceImg,
                        cv::Mat& TransformedImg,
                        cv::Mat inverseAffine,
                        InterpolationMethod mode )
{
    const int nRows = sourceImg.rows;
    const int nCols = sourceImg.cols;

    double k, q;
    int ik, iq;
    double dk, dq;

    for ( int i = 0; i < TransformedImg.rows; i++ )
    {
        for ( int j = 0; j < TransformedImg.cols; j++ )
        {
            std::tie( k, q ) = AffineTransform( inverseAffine, i, j );
            ik = mode == InterpolationMethod::nearest ? (int)std::round( k )
                                                      : (int)std::floor( k );
            dk = k - ik;
            iq = mode == InterpolationMethod::nearest ? (int)std::round( q )
                                                      : (int)std::floor( q );
            dq = q - iq;

            switch ( mode )
            {
                case InterpolationMethod::nearest:  // nearleast neighberhood
                    TransformedImg.at<uchar>( i, j ) =
                        zeroPadded<uchar>( sourceImg, ik, iq, nRows, nCols );
                    break;
                case InterpolationMethod::bilinear:
                    TransformedImg.at<uchar>( i, j ) =
                        ( ( dk * dq ) *
                              (double)zeroPadded<uchar>(
                                  sourceImg, ik + 1, iq + 1, nRows, nCols ) +
                          ( ( 1 - dk ) * dq ) *
                              (double)zeroPadded<uchar>( sourceImg, ik, iq + 1,
                                                         nRows, nCols ) +
                          ( dk * ( 1 - dq ) ) *
                              (double)zeroPadded<uchar>( sourceImg, ik + 1, iq,
                                                         nRows, nCols ) +
                          ( ( 1 - dk ) * ( 1 - dq ) ) *
                              (double)zeroPadded<uchar>( sourceImg, ik, iq,
                                                         nRows, nCols ) );
                    break;
                default:
                    break;
            }
        }
    }
}