#include "motion.hpp"

using namespace std::placeholders;

bool
isInsideRect( cv::Rect img, cv::Rect rect )
{
    auto tmp = rect & img;
    auto tmpa = tmp == rect;
    return tmpa;
}

std::vector<mv_t>
bma( cv::Mat ancher_img,
     cv::Mat tracked_img,
     const int block_size,
     obj_f objective,
     const std::vector<double>& obj_args,
     bma_f matcher,
     const std::vector<int>& bma_args )
{
    const int nrow = ancher_img.rows;
    const int ncol = ancher_img.cols;
    const int nrow_block = 1 + ( ( nrow - 1 ) / block_size );
    const int ncol_block = 1 + ( ( ncol - 1 ) / block_size );

    std::vector<mv_t> motion_vec;

    for ( int i = 0; i < nrow_block; i++ )
    {
        const int i_idx = i * block_size;
        const int i_size =
            i_idx + block_size > nrow ? nrow - i_idx : block_size;

        for ( int j = 0; j < ncol_block; j++ )
        {
            const int j_idx = j * block_size;
            const int j_size =
                j_idx + block_size > ncol ? ncol - j_idx : block_size;

            cv::Rect ancher_rect( cv::Point( j_idx, i_idx ),
                                  cv::Size( j_size, i_size ) );

            mv_t least_pos = matcher( ancher_img, tracked_img, ancher_rect,
                                      objective, obj_args, bma_args );

            motion_vec.push_back( least_pos );
        }
    }

    return motion_vec;
}

mv_t
ebma_f( cv::Mat ancher_img,
        cv::Mat tracked_img,
        const cv::Rect ancher_rect,
        obj_f objective,
        const std::vector<double>& obj_args,
        const std::vector<int>& bma_args )
{
    std::vector<std::tuple<double, int, int>> valid_error;
    cv::Mat ancher_cut = ancher_img( ancher_rect );

    for ( int y = -bma_args[0]; y < bma_args[0]; y++ )
    {
        for ( int x = -bma_args[0]; x < bma_args[0]; x++ )
        {
            cv::Rect tracked_rect = ancher_rect + cv::Point( x, y );
            if ( !isInsideRect(
                     cv::Rect( cv::Point( 0, 0 ), ancher_img.size() ),
                     tracked_rect ) )
                continue;

            valid_error.push_back( std::make_tuple(
                objective( ancher_cut, tracked_img( tracked_rect ), y, x,
                           obj_args ),
                y, x ) );
        }
    }
    // end of block match algorithm
    std::sort( valid_error.begin(), valid_error.end() );
    return std::make_tuple( ancher_rect.tl().y, ancher_rect.br().y,
                            ancher_rect.tl().x, ancher_rect.br().x,
                            std::get<1>( valid_error[0] ),  // x
                            std::get<2>( valid_error[0] ),  // y
                            (double)std::get<0>( valid_error[0] ) );
}