#include "main.hpp"

int
main( int argv, char** argc )
{
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

    auto mv_out_file = logpath / "mv.csv";

    std::ofstream mv_out;
    mv_out.open( mv_out_file.string(), std::ios::app );

    auto ref_path = inputPath / ( "cubecut1.tif" );
    auto ref_img = cv::imread( ref_path.string(), cv::IMREAD_GRAYSCALE );
    if ( ref_img.empty() )
    {
        std::cout << "image load failed!" << std::endl;
        return -1;
    }

    auto anch_path = inputPath / ( "cubecut2.tif" );
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
    const int nrow_block = 1 + ( ( nrow_ref - 1 ) / BLOCK_SIZE );
    const int ncol_block = 1 + ( ( ncol_ref - 1 ) / BLOCK_SIZE );

    if ( nrow_anch != nrow_ref || ncol_anch != ncol_ref )
    {
        std::cout << "image size is not same!" << std::endl;
        return -1;
    }

    for ( int i = 0; i < nrow_block; i++ )
    {
        for ( int j = 0; j < ncol_block; j++ )
        {
            cv::Mat ref_cut = ref_img(
                cv::Range( i * BLOCK_SIZE, ( i + 1 ) * BLOCK_SIZE > nrow_ref
                                               ? nrow_ref
                                               : ( i + 1 ) * BLOCK_SIZE ),
                cv::Range( j * BLOCK_SIZE, ( j + 1 ) * BLOCK_SIZE > ncol_ref
                                               ? ncol_ref
                                               : ( j + 1 ) * BLOCK_SIZE ) );
        }
    }

    ////////////////////////////////////

    return 0;
}
