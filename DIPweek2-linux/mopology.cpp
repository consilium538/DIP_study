#include "mopology.hpp"

cv::Mat rectSE( const int size )
{
    return cv::Mat( size, size, CV_8UC1, cv::Scalar( 255 ) );
}

cv::Mat rectSE( const int x, const int y )
{
    return cv::Mat( x, y, CV_8UC1, cv::Scalar( 255 ) );
}

cv::Mat crossSE()
{
    return ( cv::Mat_<uchar>( 3, 3 ) << 0, 255, 0, 255, 255, 255, 0, 255, 0 );
}

std::function<void( Pixel&, const int* )> singleerosion( cv::Mat& A,
                                                         cv::Mat& B )
{
    return [&]( Pixel& p, const int* position ) -> void {
        for ( int k = -( B.rows / 2 ); k <= ( B.rows / 2 ); k++ )
            for ( int l = -( B.cols / 2 ); l <= ( B.cols / 2 ); l++ )
                if ( position[0] + k >= 0 && position[0] + k < A.rows &&
                     position[1] + l >= 0 && position[1] + l < A.cols &&
                     A.at<uchar>( position[0] + k, position[1] + l ) !=
                         B.at<uchar>( ( B.rows / 2 ) - k, ( B.cols / 2 ) - l ) )
                {
                    p = 0;
                    return;
                }
        p = 255;
        return;
    };
}

// A (-) B
cv::Mat erosion( cv::Mat A, cv::Mat B )
{
    cv::Mat C = cv::Mat_<uchar>( A.size() );

    const int nRows = C.rows;
    const int nCols = C.cols;
    int p[2];

    auto f = singleerosion( A, B );

    for ( int i = 0; i < nRows; i++ )
        for ( int j = 0; j < nCols; j++ )
        {
            // C.at<uchar>( i, j ) = singleerosion( A, B, i, j );

            p[0] = i;
            p[1] = j;
            f( C.at<uchar>( i, j ), p );
        }

    return C;
}

std::function<void( Pixel&, const int* )> singledilation( cv::Mat& A,
                                                          cv::Mat& B )
{
    return [&]( Pixel& p, const int* position ) -> void {
        for ( int k = -( B.rows / 2 ); k <= ( B.rows / 2 ); k++ )
            for ( int l = -( B.cols / 2 ); l <= ( B.cols / 2 ); l++ )
                if ( position[0] + k >= 0 && position[0] + k < A.rows &&
                     position[1] + l >= 0 && position[1] + l < A.cols &&
                     A.at<uchar>( position[0] + k, position[1] + l ) ==
                         B.at<uchar>( ( B.rows / 2 ) - k, ( B.cols / 2 ) - l ) )
                {
                    p = 255;
                    return;
                }
        p = 0;
        return;
    };
}

// A (+) B
cv::Mat dilation( cv::Mat A, cv::Mat B )
{
    cv::Mat C = cv::Mat_<uchar>( A.size() );

    const int nRows = C.rows;
    const int nCols = C.cols;

    C.forEach<Pixel>( singledilation( A, B ) );

    return C;
}

cv::Mat opening( cv::Mat A, cv::Mat B )
{
    return dilation( erosion( A, B ), B );
}

cv::Mat closing( cv::Mat A, cv::Mat B )
{
    return erosion( dilation( A, B ), B );
}