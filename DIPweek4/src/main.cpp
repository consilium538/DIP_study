#include "main.hpp"

std::optional<cv::Mat>
openImg( std::filesystem::path path )
{
    cv::Mat img = cv::imread( path.string(), cv::IMREAD_GRAYSCALE );

    return img.empty() ? std::nullopt : std::make_optional( img );
}

int
main( int argv, char** argc )
{
    using namespace std::placeholders;
    using namespace std;
    using namespace cv;
    namespace fs = std::filesystem;

    const int block_size = 8;
    const int search_range = 15;

    ////////////////////////////////////

    auto config_file = fs::path( "./config.json" );
    auto inputPath = fs::path( "./srcImg/" );
    auto savepath = fs::path( "./dstImg/" );
    auto logpath = fs::path( "./log" );

    if ( !fs::exists( savepath ) )
        fs::create_directory( savepath );
    if ( !fs::exists( logpath ) )
        fs::create_directory( logpath );

    auto ancher_path = inputPath / ( "cubecut1.tif" );
    auto ancher_img = cv::imread( ancher_path.string(), cv::IMREAD_GRAYSCALE );
    if ( ancher_img.empty() )
    {
        std::cout << "image load failed!" << std::endl;
        return -1;
    }

    auto tracked_path = inputPath / ( "cubecut2.tif" );
    auto tracked_img =
        cv::imread( tracked_path.string(), cv::IMREAD_GRAYSCALE );
    if ( tracked_img.empty() )
    {
        std::cout << "image load failed!" << std::endl;
        return -1;
    }

    const int nrow_ref = ancher_img.rows;
    const int ncol_ref = ancher_img.cols;
    const int nrow_anch = tracked_img.rows;
    const int ncol_anch = tracked_img.cols;
    const int nrow_block = 1 + ( ( nrow_ref - 1 ) / block_size );
    const int ncol_block = 1 + ( ( ncol_ref - 1 ) / block_size );

    if ( nrow_anch != nrow_ref || ncol_anch != ncol_ref )
    {
        std::cout << "image size is not same!" << std::endl;
        return -1;
    }

    std::vector test_set = {
        // std::make_tuple( mad_patch, std::vector<double>(), "mad",
        //                  "MAD without constraint" )
        std::make_tuple( mad_dist, std::vector<double>( {0.1} ), "mad_c_1",
                         "MAD with distance constraint" )};

    for ( auto& it : test_set )
    {
        std::ofstream mv_out;
        mv_out.open(
            ( logpath / fmt::format( "{}{}", std::get<2>( it ), ".csv" ) )
                .string(),
            std::ios::trunc );
        mv_out << "xpos, ypos, xvec, yvec, cost\n";

        std::vector<std::tuple<int, int, int, int, int, int, double>>
            motion_vec;

        motion_vec = ebma( ancher_img, tracked_img, block_size, search_range,
                           std::get<0>( it ), std::get<1>( it ) );

        for ( auto& mv : motion_vec )
        {
            mv_out << fmt::format(
                "{0:f}, {1:f}, {2:f}, {3:f}, {4:f}\n",
                ( std::get<2>( mv ) + (double)block_size / 2 ),
                ( std::get<0>( mv ) + (double)block_size / 2 ),
                (double)std::get<5>( mv ), (double)std::get<4>( mv ),
                (double)std::get<6>( mv ) );
        }

        cv::Mat testImg = cv::Mat_<uchar>( tracked_img.size(), 0 );
        for ( auto it : motion_vec )
        {
            cv::Mat testImgPatch = tracked_img(
                cv::Range( std::get<0>( it ) + std::get<4>( it ),
                           std::get<1>( it ) + std::get<4>( it ) ),
                cv::Range( std::get<2>( it ) + std::get<5>( it ),
                           std::get<3>( it ) + std::get<5>( it ) ) );
            testImgPatch.copyTo(
                testImg( cv::Range( std::get<0>( it ), std::get<1>( it ) ),
                         cv::Range( std::get<2>( it ), std::get<3>( it ) ) ) );
        }

        mv_out << std::endl;
        std::cout << fmt::format( "end of {} computation!", std::get<2>( it ) )
                  << std::endl;
    }

    ////////////////////////////////////

    return 0;
}
