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