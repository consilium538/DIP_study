#include "ccl.hpp"

std::tuple<cv::Mat, int> grassfire_4( cv::Mat img )
{
    cv::Mat img_lable = cv::Mat_<int>( img.size(), 0 );
    cv::Mat img_marked = cv::Mat_<uchar>( img.size(), 0 );
    std::vector<std::tuple<int, int>> marker;
    std::tuple<int, int> cur_mark;
    unsigned int mark_lable = 0;

    for ( int i = 0; i < img.rows; i++ )
    {
        for ( int j = 0; j < img.cols; j++ )
        {
            if ( img.at<uchar>( i, j ) == 255 &&
                 img_marked.at<uchar>( i, j ) == 0 )
            {
                mark_lable++;
                marker.push_back( std::make_tuple( i, j ) );
                while ( !marker.empty() )
                {
                    cur_mark = marker.back();
                    marker.pop_back();
                    img_lable.at<int>( std::get<0>( cur_mark ),
                                       std::get<1>( cur_mark ) ) = mark_lable;
                    img_marked.at<uchar>( std::get<0>( cur_mark ),
                                          std::get<1>( cur_mark ) ) = 255;
                    if ( std::get<0>( cur_mark ) > 0 &&
                         img.at<uchar>( std::get<0>( cur_mark ) - 1,
                                        std::get<1>( cur_mark ) ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ) - 1,
                                               std::get<1>( cur_mark ) ) == 0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ) - 1,
                                             std::get<1>( cur_mark ) ) );

                    if ( std::get<1>( cur_mark ) > 0 &&
                         img.at<uchar>( std::get<0>( cur_mark ),
                                        std::get<1>( cur_mark ) - 1 ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ),
                                               std::get<1>( cur_mark ) - 1 ) ==
                             0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ),
                                             std::get<1>( cur_mark ) - 1 ) );

                    if ( std::get<0>( cur_mark ) < img.cols - 1 &&
                         img.at<uchar>( std::get<0>( cur_mark ) + 1,
                                        std::get<1>( cur_mark ) ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ) + 1,
                                               std::get<1>( cur_mark ) ) == 0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ) + 1,
                                             std::get<1>( cur_mark ) ) );

                    if ( std::get<1>( cur_mark ) < img.rows - 1 &&
                         img.at<uchar>( std::get<0>( cur_mark ),
                                        std::get<1>( cur_mark ) + 1 ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ),
                                               std::get<1>( cur_mark ) + 1 ) ==
                             0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ),
                                             std::get<1>( cur_mark ) + 1 ) );
                }
            }
        }
    }
    return std::make_tuple( img_lable, mark_lable );
}

std::tuple<cv::Mat, int> grassfire_8( cv::Mat img )
{
    cv::Mat img_lable = cv::Mat_<int>( img.size(), 0 );
    cv::Mat img_marked = cv::Mat_<uchar>( img.size(), 0 );
    std::vector<std::tuple<int, int>> marker;
    std::tuple<int, int> cur_mark;
    unsigned int mark_lable = 0;

    for ( int i = 0; i < img.rows; i++ )
    {
        for ( int j = 0; j < img.cols; j++ )
        {
            if ( img.at<uchar>( i, j ) == 255 &&
                 img_marked.at<uchar>( i, j ) == 0 )
            {
                mark_lable++;
                marker.push_back( std::make_tuple( i, j ) );
                while ( !marker.empty() )
                {
                    cur_mark = marker.back();
                    marker.pop_back();
                    img_lable.at<int>( std::get<0>( cur_mark ),
                                       std::get<1>( cur_mark ) ) = mark_lable;
                    img_marked.at<uchar>( std::get<0>( cur_mark ),
                                          std::get<1>( cur_mark ) ) = 255;

                    if ( std::get<0>( cur_mark ) > 0 &&
                         img.at<uchar>( std::get<0>( cur_mark ) - 1,
                                        std::get<1>( cur_mark ) ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ) - 1,
                                               std::get<1>( cur_mark ) ) == 0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ) - 1,
                                             std::get<1>( cur_mark ) ) );

                    if ( std::get<0>( cur_mark ) > 0 &&
                         std::get<1>( cur_mark ) > 0 &&
                         img.at<uchar>( std::get<0>( cur_mark ) - 1,
                                        std::get<1>( cur_mark ) - 1 ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ) - 1,
                                               std::get<1>( cur_mark ) - 1 ) ==
                             0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ) - 1,
                                             std::get<1>( cur_mark ) - 1 ) );

                    if ( std::get<1>( cur_mark ) > 0 &&
                         img.at<uchar>( std::get<0>( cur_mark ),
                                        std::get<1>( cur_mark ) - 1 ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ),
                                               std::get<1>( cur_mark ) - 1 ) ==
                             0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ),
                                             std::get<1>( cur_mark ) - 1 ) );

                    if ( std::get<0>( cur_mark ) > 0 &&
                         std::get<1>( cur_mark ) < img.cols &&
                         img.at<uchar>( std::get<0>( cur_mark ) - 1,
                                        std::get<1>( cur_mark ) + 1 ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ) - 1,
                                               std::get<1>( cur_mark ) + 1 ) ==
                             0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ) - 1,
                                             std::get<1>( cur_mark ) + 1 ) );

                    if ( std::get<0>( cur_mark ) < img.rows &&
                         img.at<uchar>( std::get<0>( cur_mark ) + 1,
                                        std::get<1>( cur_mark ) ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ) + 1,
                                               std::get<1>( cur_mark ) ) == 0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ) + 1,
                                             std::get<1>( cur_mark ) ) );

                    if ( std::get<0>( cur_mark ) < img.rows &&
                         std::get<1>( cur_mark ) < img.cols &&
                         img.at<uchar>( std::get<0>( cur_mark ) + 1,
                                        std::get<1>( cur_mark ) + 1 ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ) + 1,
                                               std::get<1>( cur_mark ) + 1 ) ==
                             0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ) + 1,
                                             std::get<1>( cur_mark ) + 1 ) );

                    if ( std::get<1>( cur_mark ) < img.cols &&
                         img.at<uchar>( std::get<0>( cur_mark ),
                                        std::get<1>( cur_mark ) + 1 ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ),
                                               std::get<1>( cur_mark ) + 1 ) ==
                             0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ),
                                             std::get<1>( cur_mark ) + 1 ) );

                    if ( std::get<0>( cur_mark ) < img.rows &&
                         std::get<1>( cur_mark ) > 0 &&
                         img.at<uchar>( std::get<0>( cur_mark ) + 1,
                                        std::get<1>( cur_mark ) - 1 ) == 255 &&
                         img_marked.at<uchar>( std::get<0>( cur_mark ) + 1,
                                               std::get<1>( cur_mark ) - 1 ) ==
                             0 )
                        marker.push_back(
                            std::make_tuple( std::get<0>( cur_mark ) + 1,
                                             std::get<1>( cur_mark ) - 1 ) );
                }
            }
        }
    }
    return std::make_tuple( img_lable, mark_lable );
}

std::tuple<cv::Mat, int> eq_label_set_8( cv::Mat img )
{
    cv::Mat img_lable = cv::Mat_<int>( img.size(), 0 );
    std::vector<int> rl_table = {0}, next_label = {0}, tail_label = {0};
    int last_label = 1;
    for ( int i = 0; i < img.rows; i++ )
    {
        for ( int j = 0; j < img.cols; j++ )
        {
            if ( img.at<uchar>( i, j ) == 255 )
            {
                // First Scan
                int mask_min = 0;
                bool hasNeighber = false;
                if ( i > 0 )  // upper mask
                {
                    if ( j > 0 && img.at<uchar>( i - 1, j - 1 ) == 255 )
                    {
                        mask_min = img_lable.at<int>( i - 1, j - 1 );
                        hasNeighber = true;
                    }
                    if ( img.at<uchar>( i - 1, j ) == 255 )
                    {
                        mask_min =
                            hasNeighber
                                ? std::min( img_lable.at<int>( i - 1, j ),
                                            mask_min )
                                : img_lable.at<int>( i - 1, j );
                        hasNeighber = true;
                    }
                    if ( j < img.cols - 1 &&
                         img.at<uchar>( i - 1, j + 1 ) == 255 )
                    {
                        mask_min =
                            hasNeighber
                                ? std::min( img_lable.at<int>( i - 1, j + 1 ),
                                            mask_min )
                                : img_lable.at<int>( i - 1, j + 1 );
                        hasNeighber = true;
                    }
                }
                if ( j > 0 && img.at<uchar>( i, j - 1 ) == 255 )
                {
                    mask_min = hasNeighber
                                   ? std::min( img_lable.at<int>( i, j - 1 ),
                                               mask_min )
                                   : img_lable.at<int>( i, j - 1 );
                    hasNeighber = true;
                }

                if ( !hasNeighber )
                {
                    img_lable.at<int>( i, j ) = last_label;
                    rl_table.push_back( last_label );
                    next_label.push_back( -1 );
                    tail_label.push_back( last_label );
                    last_label++;
                }
                else
                {
                    img_lable.at<int>( i, j ) = mask_min;
                }

                // label equivalence resolving
                if ( img_lable.at<int>( i, j - 1 ) != 0 &&
                     ( ( img_lable.at<int>( i - 1, j + 1 ) != 0 &&
                         rl_table[img_lable.at<int>( i, j - 1 )] !=
                             rl_table[img_lable.at<int>( i - 1, j + 1 )] ) ||
                       ( img_lable.at<int>( i - 1, j - 1 ) != 0 &&
                         rl_table[img_lable.at<int>( i, j - 1 )] !=
                             rl_table[img_lable.at<int>( i - 1, j - 1 )] ) ) )
                {
                    int large_value = rl_table[img_lable.at<int>( i, j - 1 )];
                    int small_value =
                        img_lable.at<int>( i - 1, j + 1 ) == 0
                            ? rl_table[img_lable.at<int>( i - 1, j - 1 )]
                            : rl_table[img_lable.at<int>( i - 1, j + 1 )];
                    rl_table;
                }
            }
        }
    }

    return std::make_tuple( img_lable, 1 );
}
