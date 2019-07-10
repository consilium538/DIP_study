#include "mopology.hpp"

cv::Mat rectSE(const int size)
{
    return cv::Mat(size, size, CV_8UC1, cv::Scalar(255));
}

cv::Mat rectSE(const int x, const int y)
{
    return cv::Mat(x, y, CV_8UC1, cv::Scalar(255));
}

uchar singleerosion( cv::Mat& A, cv::Mat& B, const int i, const int j );
uchar singledilation( cv::Mat& A, cv::Mat& B, const int i, const int j );

// A (-) B
cv::Mat erosion( cv::Mat A, cv::Mat B )
{
    cv::Mat C = cv::Mat_<uchar>( A.size() );

    const int nRows = C.rows;
    const int nCols = C.cols;

    for ( int i = 0; i < nRows; i++ )
        for ( int j = 0; j < nCols; j++ )
            C.at<uchar>( i, j ) = singleerosion( A, B, i, j );

    return C;
}

// A (+) B
cv::Mat dilation( cv::Mat A, cv::Mat B )
{
    cv::Mat C = cv::Mat_<uchar>( A.size() );

    const int nRows = C.rows;
    const int nCols = C.cols;

    for ( int i = 0; i < nRows; i++ )
        for ( int j = 0; j < nCols; j++ )
            C.at<uchar>( i, j ) = singledilation( A, B, i, j );

    return C;
}

uchar singleerosion( cv::Mat& A, cv::Mat& B, const int i, const int j )
{
    for ( int k = -( B.rows / 2 ); k <= ( B.rows / 2 ); k++ )
        for ( int l = -( B.cols / 2 ); l <= ( B.cols / 2 ); l++ )
            if ( i + k >= 0 && i + k < A.rows && j + l >= 0 && j + l < A.cols &&
                 A.at<uchar>( i + k, j + l ) !=
                     B.at<uchar>( ( B.rows / 2 ) + k, ( B.cols / 2 ) + l ) )
                return 0;

    return 255;
}

uchar singledilation( cv::Mat& A, cv::Mat& B, const int i, const int j )
{
    for ( int k = -( B.rows / 2 ); k <= ( B.rows / 2 ); k++ )
        for ( int l = -( B.cols / 2 ); l <= ( B.cols / 2 ); l++ )
            if ( i + k >= 0 && i + k < A.rows && j + l >= 0 && j + l < A.cols &&
                 A.at<uchar>( i + k, j + l ) ==
                     B.at<uchar>( ( B.rows / 2 ) + k, ( B.cols / 2 ) + l ) )
                return 255;

    return 0;
}

cv::Mat opening( cv::Mat A, cv::Mat B )
{
    return dilation(erosion(A,B),B);
}

cv::Mat closing( cv::Mat A, cv::Mat B )
{
    return erosion(dilation(A,B),B);
}