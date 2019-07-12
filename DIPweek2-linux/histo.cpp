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

double global_threshold( cv::Mat Img )
{
    auto img_hist = histogram( Img );
    int nPixels = Img.cols * Img.rows;

    double threshold = 127.0;
    double threshold_ex = 0.0;
    std::array<long int, 256> up_cumm;
    std::array<long int, 256> down_cumm;
    std::array<int, 256> num_cumm;

    up_cumm[0] = img_hist[0] * 0;
    down_cumm[255] = img_hist[255] * 255;
    num_cumm[0] = img_hist[0];

    for ( int i = 1; i < 256; i++ )
    {
        up_cumm[i] = up_cumm[i - 1] + img_hist[i] * i;
        down_cumm[255 - i] =
            down_cumm[256 - i] + img_hist[255 - i] * ( 255 - i );
        num_cumm[i] = num_cumm[i - 1] + img_hist[i];
    }

    do
    {
        threshold_ex = threshold;
        threshold =
            ( ( (double)up_cumm[(int)threshold] / num_cumm[(int)threshold] ) +
              ( (double)down_cumm[(int)threshold + 1] /
                ( nPixels - num_cumm[(int)threshold] ) ) ) /
            2.0;
    } while ( std::abs( threshold - threshold_ex ) > 0.000001 );

    return threshold;
}

double otsu_threshold( cv::Mat Img )
{
    double threshold;

    auto hist = histogram( Img );
    std::array<double, 256> prob, prob_cum, mean, mean_cum, var_bc;
    for ( int i = 0; i < 256; i++ )
    {
        prob[i] = double( hist[i] ) / ( Img.cols * Img.rows );
        prob_cum[i] = prob[i] + ( i == 0 ? 0 : prob_cum[i - 1] );
        mean[i] = double( i ) / prob[i] + ( i == 0 ? 0 : mean[i - 1] );
        mean_cum[i] = mean[i] / prob_cum[i];
    }
    auto mean_g = mean[255];

    for ( int i = 0; i < 256; i++ )
    {
        var_bc[i] = ( std::pow( mean_g * prob_cum[i] - mean_cum[i], 2 ) ) /
                    ( prob_cum[i] * ( 1 - prob_cum[i] ) );
    }

    std::vector<int> largest_label;

    return threshold;
}