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

std::array<int, 256> indexed_histogram( cv::Mat& img, cv::Mat index )
{
    CV_Assert( img.depth() == CV_8U );
    CV_Assert( img.channels() == 1 );
    CV_Assert( index.depth() == CV_8U );
    CV_Assert( index.channels() == 1 );
    CV_Assert( img.rows == index.rows );
    CV_Assert( img.cols == index.cols );
    std::array<int, 256> hist;

    uchar *p, *pi;

    int nRows = img.rows;
    int nCols = img.cols;

    if ( img.isContinuous() && index.isContinuous() )
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
        pi = index.ptr<uchar>( i );
        for ( int j = 0; j < nCols; j++ )
        {
            if ( pi[j] != 0 )
                ++hist[p[j]];
        }
    }

    return hist;
}

std::array<int, 1024> histogram_ex( cv::Mat& img )
{
    CV_Assert( img.channels() == 1 );
    std::array<int, 1024> hist;
    cv::Mat img_short;
    img.convertTo(img_short, CV_16U);

    unsigned short *p;

    int nRows = img.rows;
    int nCols = img.cols;

    if ( img.isContinuous() )
    {
        nCols *= nRows;
        nRows = 1;
    }

    for ( int i = 0; i < 1024; i++ )
    {
        hist[i] = 0;
    }

    for ( int i = 0; i < nRows; i++ )
    {
        p = img_short.ptr<unsigned short>( i );
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

double otsu_threshold_indexed( cv::Mat Img, cv::Mat Index )
{
    auto hist = indexed_histogram( Img, Index );
    int pixels = std::accumulate(hist.begin(),hist.end(),0);
    std::array<double, 256> prob, prob_cum, mean, mean_cum, var_bc;
    for ( int i = 0; i < 256; i++ )
    {
        prob[i] = double( hist[i] ) / ( pixels );
        prob_cum[i] = prob[i] + ( i == 0 ? 0 : prob_cum[i - 1] );
        mean[i] = double( i ) * prob[i] + ( i == 0 ? 0 : mean[i - 1] );
        mean_cum[i] = prob_cum[i] > 0 ? mean[i] / prob_cum[i] : 0;
    }
    auto mean_g = mean[255];

    double tmp;

    for ( int i = 0; i < 256; i++ )
    {
        tmp = prob_cum[i] * ( 1 - prob_cum[i] );

        var_bc[i] = tmp > 0
                        ? ( std::pow( mean_g * prob_cum[i] - mean[i], 2 ) ) /
                              ( prob_cum[i] * ( 1 - prob_cum[i] ) )
                        : 0;
    }

    std::vector<int> largest_label;
    double max_var_bc;

    for ( int i = 0; i < 256; i++ )
    {
        if ( var_bc[i] > max_var_bc )
        {
            max_var_bc = var_bc[i];
            largest_label.clear();
            largest_label.push_back( i );
        }
        else if ( var_bc[i] == max_var_bc )
        {
            largest_label.push_back( i );
        }
    }

    if ( largest_label.size() == 1 )
        return largest_label[0];
    else
        return (double)std::accumulate( largest_label.begin(),
                                        largest_label.end(), 0 ) /
               largest_label.size();
}

double otsu_threshold( cv::Mat Img )
{
    auto hist = histogram( Img );
    std::array<double, 256> prob, prob_cum, mean, mean_cum, var_bc;
    for ( int i = 0; i < 256; i++ )
    {
        prob[i] = double( hist[i] ) / ( Img.cols * Img.rows );
        prob_cum[i] = prob[i] + ( i == 0 ? 0 : prob_cum[i - 1] );
        mean[i] = double( i ) * prob[i] + ( i == 0 ? 0 : mean[i - 1] );
        mean_cum[i] = prob_cum[i] > 0 ? mean[i] / prob_cum[i] : 0;
    }
    auto mean_g = mean[255];

    double tmp;

    for ( int i = 0; i < 256; i++ )
    {
        tmp = prob_cum[i] * ( 1 - prob_cum[i] );

        var_bc[i] = tmp > 0
                        ? ( std::pow( mean_g * prob_cum[i] - mean[i], 2 ) ) /
                              ( prob_cum[i] * ( 1 - prob_cum[i] ) )
                        : 0;
    }

    std::vector<int> largest_label;
    double max_var_bc;

    for ( int i = 0; i < 256; i++ )
    {
        if ( var_bc[i] > max_var_bc )
        {
            max_var_bc = var_bc[i];
            largest_label.clear();
            largest_label.push_back( i );
        }
        else if ( var_bc[i] == max_var_bc )
        {
            largest_label.push_back( i );
        }
    }

    if ( largest_label.size() == 1 )
        return largest_label[0];
    else
        return (double)std::accumulate( largest_label.begin(),
                                        largest_label.end(), 0 ) /
               largest_label.size();
}
