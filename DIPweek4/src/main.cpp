#include "main.hpp"

template <typename Iterable>
Json::Value
iterable2json( Iterable const& cont )
{
    Json::Value v;
    for ( auto&& element : cont )
    {
        v.append( element );
    }
    return v;
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

int
main( int argv, char** argc )
{
    using namespace std::placeholders;
    Json::Value log_root = {};
    auto start_time = std::time( nullptr );
    log_root["start_datetime"] =
        fmt::format( "{:%c}", *std::localtime( &start_time ) );

    const int block_size = 8;
    const int search_range = 15;

    Json::Value block_param = {};
    block_param["block_size"] = block_size;
    block_param["search_range"] = search_range;
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

    Json::Value file_loc = {};

    auto ref_path = inputPath / ( "street1.tif" );
    auto ref_img = cv::imread( ref_path.string(), cv::IMREAD_GRAYSCALE );
    if ( ref_img.empty() )
    {
        std::cout << "image load failed!" << std::endl;
        return -1;
    }
    file_loc["anchor_img_path"] = ref_path.string();

    auto anch_path = inputPath / ( "street2.tif" );
    auto anch_img = cv::imread( anch_path.string(), cv::IMREAD_GRAYSCALE );
    if ( anch_img.empty() )
    {
        std::cout << "image load failed!" << std::endl;
        return -1;
    }
    file_loc["tracked_img_path"] = anch_path.string();

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
//          std::make_tuple( mad_patch, std::vector<double>(), "mad",
//                           "MAD without constraint" )
        std::make_tuple( mad_dist, std::vector<double>( {0.1} ), "mad_c_1",
                         "MAD with distance constraint" )
    };

    for ( auto it : test_set )
    {
        Json::Value test_env = {};
        test_env["block_param"] = block_param;
        test_env["file_loc"] = file_loc;
        test_env["method"] = std::get<3>( it );
        test_env["mt"] = std::get<2>( it );
        test_env["value"] = iterable2json( std::get<1>( it ) );

        std::ofstream mv_out;
        mv_out.open(
            ( logpath / fmt::format( "{}{}", std::get<2>( it ), ".csv" ) )
                .string(),
            std::ios::trunc );
        mv_out << "xpos, ypos, xvec, yvec, cost\n";

        std::vector<std::tuple<int,int,int,int,int,int,double>>
            motion_vec;

        for ( int i = 0; i < nrow_block; i++ )
        {
            const int i_idx = i * block_size;
            const int i_size =
                i_idx + block_size > nrow_ref ? nrow_ref : i_idx + block_size;

            for ( int j = 0; j < ncol_block; j++ )
            {
                const int j_idx = j * block_size;
                const int j_size = j_idx + block_size > ncol_ref
                                       ? ncol_ref
                                       : j_idx + block_size;
                cv::Mat ref_cut = ref_img( cv::Range( i_idx, i_size ),
                                           cv::Range( j_idx, j_size ) );

                std::vector<std::tuple<double, int, int>> valid_error;

                for ( int x = -( (int)search_range ); x < search_range; x++ )
                {
                    for ( int y = -( (int)search_range ); y < search_range;
                          y++ )
                    {
                        std::optional<cv::Mat> anch_cut =
                            isInside( nrow_anch, ncol_anch, i_idx + x,
                                      j_idx + y, ref_cut.rows, ref_cut.cols )
                                ? make_optional( anch_img(
                                      cv::Range( i_idx + x,
                                                 i_idx + x + ref_cut.rows ),
                                      cv::Range( j_idx + y,
                                                 j_idx + y + ref_cut.cols ) ) )
                                : std::nullopt;

                        auto method = std::bind( std::get<0>( it ), ref_cut, _1,
                                                 x, y, std::get<1>( it ) );

                        std::optional<double> error_local =
                            anch_cut.has_value() ? std::make_optional( method(
                                                       anch_cut.value() ) )
                                                 : std::nullopt;

                        if ( error_local.has_value() )
                            valid_error.push_back(
                                std::make_tuple( error_local.value(), x, y ) );
                    }
                }

                std::sort( valid_error.begin(), valid_error.end() );
                auto least_pos = std::make_tuple(
                    i_idx, i_size, j_idx, j_size,
                    std::get<1>( valid_error[0] ), // x
                    std::get<2>( valid_error[0] ), // y
                    (double)std::get<0>( valid_error[0] ) );

                mv_out << fmt::format(
                    "{0:f}, {1:f}, {2:f}, {3:f}, {4:f}\n",
                    ( j_idx + (double)block_size / 2 ),
                    ( i_idx + (double)block_size / 2 ),
                    (double)std::get<2>( valid_error[0] ),
                    (double)std::get<1>( valid_error[0] ),
                    (double)std::get<0>( valid_error[0] ));
                motion_vec.push_back(least_pos);
            }
        }

        cv::Mat testImg = cv::Mat_<uchar>(anch_img.size(),0);
        for ( auto it : motion_vec )
        {
            cv::Mat testImgPatch = anch_img(
                cv::Range(
                    std::get<0>( it ) + std::get<4>( it ),
                    std::get<1>( it ) + std::get<4>( it )
                ),
                cv::Range(
                    std::get<2>( it ) + std::get<5>( it ),
                    std::get<3>( it ) + std::get<5>( it )
                ) );
            testImgPatch.copyTo( testImg(
                cv::Range(
                    std::get<0>( it ), std::get<1>( it )
                ),
                cv::Range(
                    std::get<2>( it ), std::get<3>( it )
                )
            ) );
        }

        mv_out << std::endl;
        std::cout << fmt::format( "end of {} computation!", std::get<2>( it ) )
                  << std::endl;
        log_root["tests"].append( test_env );
    }

    std::ofstream meta_out;
    meta_out.open( ( logpath / "meta.json" ).string(), std::ios::trunc );
    meta_out << log_root << std::endl;

    ////////////////////////////////////

    return 0;
}
