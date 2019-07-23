#include "ccl.hpp"

std::tuple<cv::Mat, int> grassfire_4( cv::Mat img )
{
    cv::Mat img_label = cv::Mat_<int>( img.size(), 0 );
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
                    img_label.at<int>( std::get<0>( cur_mark ),
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
    return std::make_tuple( img_label, mark_lable );
}

std::tuple<cv::Mat, int> grassfire_8( cv::Mat img )
{
    cv::Mat img_label = cv::Mat_<int>( img.size(), 0 );
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
                    img_label.at<int>( std::get<0>( cur_mark ),
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
    return std::make_tuple( img_label, mark_lable );
}

std::tuple<cv::Mat, int> eq_label_set_8( cv::Mat img )
{
    cv::Mat img_label = cv::Mat_<int>( img.size(), 0 );
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

                const uchar upleftpixel =
                    ( ( i > 0 && j > 0 ) ? img.at<uchar>( i - 1, j - 1 ) : 0 );
                const uchar upperpixel =
                    ( ( i > 0 ) ? img.at<uchar>( i - 1, j ) : 0 );
                const uchar uprightpixel = ( ( i > 0 && j < img.cols - 1 )
                                                 ? img.at<uchar>( i - 1, j + 1 )
                                                 : 0 );
                const uchar leftpixel =
                    ( ( j > 0 ) ? img.at<uchar>( i, j - 1 ) : 0 );

                const int uplefttable =
                    ( ( i > 0 && j > 0 ) ? img_label.at<int>( i - 1, j - 1 )
                                         : 0 );
                const int uppertable =
                    ( ( i > 0 ) ? img_label.at<int>( i - 1, j ) : 0 );
                const int uprighttable =
                    ( ( i > 0 && j < img.cols - 1 )
                          ? img_label.at<int>( i - 1, j + 1 )
                          : 0 );
                const int lefttable =
                    ( ( j > 0 ) ? img_label.at<int>( i, j - 1 ) : 0 );

                if ( upleftpixel == 255 )
                {
                    mask_min = uplefttable;
                    hasNeighber = true;
                }
                if ( upperpixel == 255 )
                {
                    mask_min = hasNeighber ? std::min( uppertable, mask_min )
                                           : uppertable;
                    hasNeighber = true;
                }
                if ( uprightpixel == 255 )
                {
                    mask_min = hasNeighber ? std::min( uprighttable, mask_min )
                                           : uprighttable;
                    hasNeighber = true;
                }
                if ( leftpixel == 255 )
                {
                    mask_min = hasNeighber ? std::min( lefttable, mask_min )
                                           : lefttable;
                    hasNeighber = true;
                }

                if ( !hasNeighber )
                {
                    img_label.at<int>( i, j ) = last_label;
                    rl_table.push_back( last_label );
                    next_label.push_back( -1 );
                    tail_label.push_back( last_label );
                    last_label++;
                }
                else
                {
                    img_label.at<int>( i, j ) = mask_min;
                }

                // label equivalence resolving
                const int right_value = rl_table[uprighttable];
                const int left_value = uplefttable == 0 ? rl_table[lefttable]
                                                        : rl_table[uplefttable];
                if ( right_value != 0 && left_value != 0 &&
                     right_value != left_value )
                {
                    const int large_value =
                        right_value < left_value ? left_value : right_value;
                    const int small_value =
                        right_value < left_value ? right_value : left_value;
                    for ( int i = large_value; i != -1; i = next_label[i] )
                        rl_table[i] = small_value;
                    next_label[tail_label[small_value]] = large_value;
                    tail_label[small_value] = tail_label[large_value];
                }
            }
        }
    }

    // second scan, not need to rester scan
    cv::Mat img_label_final = img_label.clone();
    img_label_final.forEach<int>(
        [&]( int& p, const int* i ) { p = rl_table[p]; } );

    std::set<int> label_count;
    for ( auto it : rl_table )
        label_count.insert( it );

    return std::make_tuple( img_label_final, label_count.size() - 1 );
}

std::tuple<cv::Mat, int> eq_label_set_4( cv::Mat img )
{
    cv::Mat img_label = cv::Mat_<int>( img.size(), 0 );
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
                const uchar upperpixel =
                    ( ( i > 0 ) ? img.at<uchar>( i - 1, j ) : 0 );
                const uchar leftpixel =
                    ( ( j > 0 ) ? img.at<uchar>( i, j - 1 ) : 0 );

                const int uppertable =
                    ( ( i > 0 ) ? img_label.at<int>( i - 1, j ) : 0 );
                const int lefttable =
                    ( ( j > 0 ) ? img_label.at<int>( i, j - 1 ) : 0 );
                if ( upperpixel == 255 )
                {
                    mask_min = uppertable;
                    hasNeighber = true;
                }
                if ( leftpixel == 255 )
                {
                    mask_min = hasNeighber ? std::min( lefttable, mask_min )
                                           : lefttable;
                    hasNeighber = true;
                }

                if ( !hasNeighber )
                {
                    img_label.at<int>( i, j ) = last_label;
                    rl_table.push_back( last_label );
                    next_label.push_back( -1 );
                    tail_label.push_back( last_label );
                    last_label++;
                }
                else
                {
                    img_label.at<int>( i, j ) = mask_min;
                }

                // label equivalence resolving
                const int right_value = rl_table[uppertable];
                const int left_value = rl_table[lefttable];
                if ( right_value != 0 && left_value != 0 &&
                     right_value != left_value )
                {
                    const int large_value =
                        right_value < left_value ? left_value : right_value;
                    const int small_value =
                        right_value < left_value ? right_value : left_value;
                    for ( int i = large_value; i != -1; i = next_label[i] )
                        rl_table[i] = small_value;
                    next_label[tail_label[small_value]] = large_value;
                    tail_label[small_value] = tail_label[large_value];
                }
            }
        }
    }

    // second scan, not need to rester scan
    cv::Mat img_label_final = img_label.clone();
    img_label_final.forEach<int>(
        [&]( int& p, const int* i ) { p = rl_table[p]; } );

    std::set<int> label_count;
    for ( auto it : rl_table )
        label_count.insert( it );

    return std::make_tuple( img_label_final, label_count.size() - 1 );
}
