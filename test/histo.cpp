#include "histo.hpp"

// template<uchar>
std::array<int, 256> histogram( cv::Mat& img )
{
    CV_Assert( img.depth() == CV_8U );
    CV_Assert( img.channels() == 1 );
    std::array<int, 256> hist;

    uchar* p;

    int nRows = img.rows;
    int nCols = img.cols;

    if ( img.isContinuous() )
    {
        nCols *= nRows;
        nRows = 1;
    }

    for ( int i = 0; i < 256; i++ )
    {
        hist[i] = 0;
    }

    for ( int i = 0; i < nRows; i++ )
    {
        p = img.ptr<uchar>( i );
        for ( int j = 0; j < nCols; j++ )
        {
            ++hist[p[j]];
        }
    }

    return hist;
}