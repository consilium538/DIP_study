#include "conv.hpp"

cv::Mat conv2d( cv::Mat large, cv::Mat small, Padding method )
{
    // CV_Assert(large.type() == CV_64F);
    cv::Mat conv;
    conv = cv::Mat( large.size(), CV_64FC3 );
    int nRow = large.rows;
    int nCol = large.cols;
    int nRowS = small.rows;
    int nColS = small.cols;
    cv::Vec3d* ptr;
    cv::Scalar sum;
    cv::Mat focusMat = cv::Mat( small.size(), CV_64FC3 );
    cv::Mat small_a;
    if ( small.channels() == 1 )
        cv::merge( std::array<cv::Mat, 3>{small, small, small}, small_a );
    else  // 3 channels
        small_a = small;

    // int nRow = method == 1 ? large.rows : large.rows + small.rows;

    for ( int i = 0; i < nRow; i++ )
    {
        ptr = conv.ptr<cv::Vec3d>( i );
        for ( int j = 0; j < nCol; j++ )
        {
            for ( int k = 0; k < nRowS; k++ )
            {
                int k_centered;

                k_centered = i + k - nRowS / 2;
                switch ( method )
                {
                    case Padding::zero:  // zero padding
                        for ( int l = 0; l < nColS; l++ )
                        {
                            int l_centered;

                            l_centered = j + l - nColS / 2;
                            // if k_centered lower bound over
                            if ( k_centered < 0 )
                                focusMat.at<cv::Vec3d>( k, l ) = {0.0, 0.0,
                                                                  0.0};
                            // if k_centered upper bound over
                            else if ( k_centered >= nRow )
                                focusMat.at<cv::Vec3d>( k, l ) = {0.0, 0.0,
                                                                  0.0};
                            // if l_centered lower bound over
                            else if ( l_centered < 0 )
                                focusMat.at<cv::Vec3d>( k, l ) = {0.0, 0.0,
                                                                  0.0};
                            // if l_centered upper bound over
                            else if ( l_centered >= nRow )
                                focusMat.at<cv::Vec3d>( k, l ) = {0.0, 0.0,
                                                                  0.0};
                            else
                                focusMat.at<cv::Vec3d>( k, l ) =
                                    large.at<cv::Vec3d>( k_centered,
                                                         l_centered );
                        }
                        break;

                    case Padding::replicate:  // replicate padding
                        if ( k_centered < 0 )
                            k_centered = 0;
                        else if ( k_centered >= nRow )
                            k_centered = nRow - 1;
                        for ( int l = 0; l < nColS; l++ )
                        {
                            int l_centered;

                            l_centered = j + l - nColS / 2;

                            if ( l_centered < 0 )
                                l_centered = 0;
                            else if ( l_centered >= nCol )
                                l_centered = nCol - 1;

                            focusMat.at<cv::Vec3d>( k, l ) =
                                large.at<cv::Vec3d>( k_centered, l_centered );
                        }
                        break;

                    case Padding::mirror:  // mirror padding
                        if ( k_centered < 0 )
                            k_centered = 1 - k_centered;
                        else if ( k_centered >= nRow )
                            k_centered = 2 * nRow - k_centered - 1;
                        for ( int l = 0; l < nColS; l++ )
                        {
                            int l_centered;

                            l_centered = j + l - nColS / 2;

                            if ( l_centered < 0 )
                                l_centered = 1 - l_centered;
                            else if ( l_centered >= nCol )
                                l_centered = 2 * nCol - l_centered - 1;

                            focusMat.at<cv::Vec3d>( k, l ) =
                                large.at<cv::Vec3d>( k_centered, l_centered );
                        }
                        break;
                    default:
                        focusMat = cv::Scalar::all( 0 );
                        break;
                }
            }
            sum = cv::sum( focusMat.mul( small_a ) );
            ptr[j][0] = sum[0];
            ptr[j][1] = sum[1];
            ptr[j][2] = sum[2];
        }
    }

    return conv.clone();
}

cv::Mat box_filter( int size )
{
    cv::Mat filter =
        cv::Mat::ones( size, size, CV_64F ) / ( (double)size * size );

    return filter.clone();
}

cv::Mat gaussian_filter( int size, double deviation )
{
    cv::Mat filter = cv::Mat( size, size, CV_64F );
    double tmp;
    for ( int i = 0; i < size; i++ )
    {
        for ( int j = 0; j < size; j++ )
        {
            tmp =
                std::exp( -( std::pow( (double)( i - ( size - 1 ) / 2 ), 2 ) +
                             std::pow( (double)( j - ( size - 1 ) / 2 ), 2 ) ) /
                          2 * std::pow( deviation, 2 ) );
            filter.at<double>( i, j ) = tmp;
        }
    }

    return filter / cv::sum( filter )[0];
}

cv::Mat laplacian_filter( LaplaceOption way )
{
    cv::Mat filter;
    switch ( way )
    {
        case LaplaceOption::four:
            filter = ( cv::Mat_<double>( 3, 3 ) << 0.0, -1.0, 0.0, -1.0, 4.0,
                       -1.0, 0.0, -1.0, 0.0 );
            break;
        case LaplaceOption::eight:
            filter = ( cv::Mat_<double>( 3, 3 ) << -1.0, -1.0, -1.0, -1.0, 8.0,
                       -1.0, -1.0, -1.0, -1.0 );
            break;
        default:
            filter = ( cv::Mat_<double>( 3, 3 ) << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0 );
            break;
    }

    return filter.clone();
}

cv::Mat sobel_filter( SobelOption dir )
{
    cv::Mat filter;
    switch ( dir )
    {
        case SobelOption::xdir:
            filter = ( cv::Mat_<double>( 3, 3 ) << -1.0, -2.0, -1.0, 0.0, 0.0,
                       0.0, 1.0, 2.0, 1.0 );
            break;
        case SobelOption::ydir:
            filter = ( cv::Mat_<double>( 3, 3 ) << -1.0, 0.0, 1.0, -2.0, 0.0,
                       2.0, -1.0, 0.0, 1.0 );
            break;
        default:
            filter = ( cv::Mat_<double>( 3, 3 ) << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0 );
            break;
    }

    return filter.clone();
}

cv::Mat adaptive_median( cv::Mat orig, int maxWindowSize )
{
    cv::Mat ret = cv::Mat( orig.size(), orig.type() );
    cv::Mat window, splitwindow[3];
    uchar tmp;

    const int nRow = orig.rows;
    const int nCol = orig.cols;

    unsigned char bMax[3];
    unsigned char bMin[3];
    unsigned char bMedian[3];

    int w;
    std::vector<uchar> tmparr;

    for ( int i = 0; i < nRow; i++ )
    {
        for ( int j = 0; j < nCol; j++ )
        {
            w = 1;
            do
            {  // j + l - nColS / 2
                window =
                    ret( cv::Rect( std::max( i - w, 0 ), std::max( j - w, 0 ),
                                   std::min( 2 * w + 1, nRow - i + w ),
                                   std::min( 2 * w + 1, nCol - j + w ) ) );
                cv::split( window, splitwindow );
                for ( int k = 0; k < 2; k++ )
                {
                    tmparr.clear();
                    tmparr.assign( splitwindow[k].begin<uchar>,
                                   splitwindow[k].end<uchar> );
                    std::sort( tmparr.begin(), tmparr.end() );

                    bMax[k] = tmparr.back();
                    bMin[k] = tmparr.front();
                    bMedian[k] = tmparr.size();
                }
                // level A
            } while ( w < maxWindowSize );
            // level B
        }
    }

    return ret;
}
