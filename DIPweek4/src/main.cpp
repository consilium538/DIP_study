#include "main.hpp"

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

int
main( int argv, char** argc )
{
    const int block_size = 4;
    const int search_range = 7;
    ////////////////////////////////////

    using namespace std;
    using namespace cv;
    namespace fs = std::filesystem;

    auto config_file = fs::path( "./config.json" );
    auto inputPath = fs::path( "./srcImg/" );
    auto savepath = fs::path( "./dstImg/" );
    auto logpath = fs::path( "./log" );

    if ( !fs::exists( config_file ) )
    {
        std::cout << "no configure file!" << std::endl;

        return 0;
    }

    if ( !fs::exists( savepath ) )
        fs::create_directory( savepath );
    if ( !fs::exists( logpath ) )
        fs::create_directory( logpath );

    auto ref_path = inputPath / ( "test1.tif" );
    auto ref_img = cv::imread( ref_path.string(), cv::IMREAD_GRAYSCALE );
    if ( ref_img.empty() )
    {
        std::cout << "image load failed!" << std::endl;
        return -1;
    }

    auto anch_path = inputPath / ( "test2.tif" );
    auto anch_img = cv::imread( anch_path.string(), cv::IMREAD_GRAYSCALE );
    if ( anch_img.empty() )
    {
        std::cout << "image load failed!" << std::endl;
        return -1;
    }

    const int nrow_ref = ref_img.rows;
    const int ncol_ref = ref_img.cols;
    const int nrow_anch = anch_img.rows;
    const int ncol_anch = anch_img.cols;
    const int nrow_block = 1 + ( ( nrow_ref - 1 ) / block_size );
    const int ncol_block = 1 + ( ( ncol_ref - 1 ) / block_size );

    if ( nrow_anch != nrow_ref || ncol_anch != ncol_ref )
    {
        std::cout << "image size is not same!" << std::endl;
        return -1;
    }

    std::vector test_set = {
        std::make_tuple(mad_patch,std::vector<double>(),"mv_t.csv"),
        std::make_tuple(mad_dist,std::vector<double>({1.0}),"mv_c_1_t.csv")
    };

    for ( auto it : test_set )
    {
        std::ofstream mv_out;
        mv_out.open( ( logpath / std::get<2>(it) ).string(), std::ios::trunc );
        mv_out << "xpos, ypos, xvec, yvec, cost\n";

        for ( int i = 0; i < nrow_block; i++ )
        {
            const int i_idx = i * block_size;
            for ( int j = 0; j < ncol_block; j++ )
            {
                const int j_idx = j * block_size;
                cv::Mat ref_cut =
                    ref_img( cv::Range( i_idx, i_idx + block_size > nrow_ref
                                                   ? nrow_ref
                                                   : i_idx + block_size ),
                             cv::Range( j_idx, j_idx + block_size > ncol_ref
                                                   ? ncol_ref
                                                   : j_idx + block_size ) );

                std::vector<std::tuple<double, int, int>> valid_error;

                for ( int x = -( (int)search_range ); x < search_range; x++ )
                {
                    for ( int y = -( (int)search_range ); y < search_range; y++ )
                    {
                        std::optional<cv::Mat> anch_cut =
                            isInside( nrow_anch, ncol_anch, i_idx + x, j_idx + y,
                                      ref_cut.rows, ref_cut.cols )
                                ? make_optional( anch_img(
                                      cv::Range( i_idx + x,
                                                 i_idx + x + ref_cut.rows ),
                                      cv::Range( j_idx + y,
                                                 j_idx + y + ref_cut.cols ) ) )
                                : std::nullopt;

                        int a = 1;
                        std::optional<double> error_local =
                            anch_cut.has_value()
                                ? std::make_optional(
                                      std::get<0>(it)( ref_cut, anch_cut.value(), x, y,
                                                std::get<1>(it) ) )
                                : std::nullopt;

                        if ( error_local.has_value() )
                            valid_error.push_back(
                                std::make_tuple( error_local.value(), x, y ) );
                    }
                }

                std::sort( valid_error.begin(), valid_error.end() );
                mv_out << fmt::format(
                    "{0:.2f}, {1:.2f}, {2:.2f}, {3:.2f}, {4:.4f}\n",
                    ( j_idx + (double)block_size / 2 ), // ncol_ref,
                    ( i_idx + (double)block_size / 2 ), // nrow_ref,
                    (double)std::get<2>( valid_error[0] ),
                    (double)std::get<1>( valid_error[0] ),
                    std::get<0>( valid_error[0] ) );
            }
        }

        mv_out << std::endl;
        std::cout << fmt::format("end of {} computation!",std::get<2>(it)) << std::endl;
    }

    ////////////////////////////////////

    return 0;
}
