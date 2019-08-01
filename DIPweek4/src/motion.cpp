#include "motion.hpp"

using namespace std::placeholders;

template <typename T, typename F>
auto
conv_optional( const std::optional<T>& o, F&& f ) -> std::optional<
    std::decay_t<decltype( std::invoke( std::forward<F>( f ), *o ) )>>
{
    if ( o )
        return std::invoke( std::forward<F>( f ), *o );
    else
        return std::nullopt;
}

template <typename T, typename F>
auto
convert_optional( std::optional<T>& o, F&& f ) -> std::optional<
    std::decay_t<decltype( std::invoke( std::forward<F>( f ), *o ) )>>
{
    if ( o )
        return std::invoke( std::forward<F>( f ), *o );
    else
        return std::nullopt;
}

template <typename T, typename F>
auto
convert_optional( std::optional<T>&& o, F&& f )
    -> std::optional<std::decay_t<decltype( std::invoke( std::forward<F>( f ),
                                                         *std::move( o ) ) )>>
{
    if ( o )
        return std::invoke( std::forward<F>( f ), *std::move( o ) );
    else
        return std::nullopt;
}

bool
isInside( const int nrow_anch,
          const int ncol_anch,
          const int i_idx,
          const int j_idx,
          const int size_x,
          const int size_y )
{
    if ( i_idx >= 0 && j_idx >= 0 && i_idx <= nrow_anch - size_x &&
         j_idx <= ncol_anch - size_y )
        return true;
    else
        return false;
}

std::vector<std::tuple<int, int, int, int, int, int, double>>
ebma( cv::Mat ancher_img,
      cv::Mat tracked_img,
      const int block_size,
      const int search_range,
      obj_f objective,
      const std::vector<double>& obj_args )
{
    const int nrow = ancher_img.rows;
    const int ncol = ancher_img.cols;
    const int nrow_block = 1 + ( ( nrow - 1 ) / block_size );
    const int ncol_block = 1 + ( ( ncol - 1 ) / block_size );

    std::vector<std::tuple<int, int, int, int, int, int, double>> motion_vec;

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

            cv::Rect ancher_rect( cv::Point(j_idx,i_idx), cv::Size(j_size,i_size) );
            cv::Mat ancher_cut = ancher_img( ancher_rect );

            std::vector<std::tuple<double, int, int>> valid_error;
// cv::Mat ancher_img, cv::Mat tracked_img, const int block_size, const cv::Rect orig_pos
// obj_f objective, const std::vector<double>& obj_args
// bma_f matcher, const std::vector<int>& bma_args
            for ( int x = -( (int)search_range ); x < search_range; x++ )
            {
                for ( int y = -( (int)search_range ); y < search_range; y++ )
                {
                    auto method =
                        std::bind( objective, ancher_cut, _1, x, y, obj_args );
                    const bool isinside =
                        isInside( nrow, ncol, i_idx + x, j_idx + y,
                                  ancher_cut.rows, ancher_cut.cols );

                    std::optional<cv::Mat> tracked_cut =
                        isinside ? std::make_optional( tracked_img(
                                       cv::Range( i_idx + x, i_idx + i_size + x ),
                                       cv::Range( j_idx + y, j_idx + j_size + y ) ) )
                                 : std::nullopt;

                    std::optional<double> error_local =
                        conv_optional( tracked_cut, method );

                    if ( error_local.has_value() )
                        valid_error.push_back(
                            std::make_tuple( error_local.value(), x, y ) );
                }
            }
// end of block match algorithm
            std::sort( valid_error.begin(), valid_error.end() );
            auto least_pos =
                std::make_tuple( i_idx, i_size + i_idx, j_idx, j_size + j_idx,
                                 std::get<1>( valid_error[0] ),  // x
                                 std::get<2>( valid_error[0] ),  // y
                                 (double)std::get<0>( valid_error[0] ) );

            motion_vec.push_back( least_pos );
        }
    }

    return motion_vec;
}
