#include "main.hpp"

int
main()
{
    namespace fs = std::filesystem;

    std::vector test_set = {
        // std::make_tuple( "foreman", 8, "tss", 15, "mad_dist", 0.1 ),
        std::make_tuple( "foreman", 4, "tdls", 15, "mad_dist", 0.1 ),
        // std::make_tuple( "foreman", 8, "ebma", 7, "mad_dist", 0.1 ),
        // std::make_tuple( "foreman", 8, "ebma", 3, "mad_dist", 0.1 ),
    };

    cv::VideoWriter orig_forman;
    cv::Mat sample_forman = cv::imread(
        "/home/dhej1/Downloads/Telegram Desktop/"
        // "/home/consilium538/Downloads/"
        "2014_DIP/FOREMAN(300)/FOREMAN000.tif" );

    auto img_iter = fs::directory_iterator(
        "/home/dhej1/Downloads/Telegram Desktop/2014_DIP/FOREMAN(300)" );
        // "/home/consilium538/Downloads/2014_DIP/FOREMAN(300)" );
    std::vector img_vec( begin( img_iter ), end( img_iter ) );
    std::sort( img_vec.begin(), img_vec.end() );
    cv::Mat img_before;

    for ( auto& [img_str, block_size, bma_str, bma_args, obj_str, obj_args] : test_set )
    {
        std::string bma_arg_str = fmt::format( "{}", bma_args );
        std::string obj_arg_str = fmt::format( "{}", obj_args );

        std::string option_str =
            fmt::format( "{}({})_{}({})_{}({})", img_str, block_size, bma_str,
                         bma_arg_str, obj_str, obj_arg_str );
        orig_forman.open( fmt::format("./forman_4_{}.mp4",option_str),
                          (int)cv::VideoWriter::fourcc( 'a', 'v', 'c', '1' ),
                          30.0, sample_forman.size(), false );
        
        std::cout << "start of " << option_str << std::endl;

        int nCount = 0;
        for ( auto& img_path : img_vec )
        {
            cv::Mat img =
                cv::imread( img_path.path().string(), cv::IMREAD_GRAYSCALE );
            if ( img_before.empty() | nCount % 4 == 0 )
            {
                orig_forman.write( img );
                img_before = img;
            }
            else
            {
                std::vector<mv_t> motion_vec;

                motion_vec = bma( img, img_before, block_size, obj_map[obj_str],
                                  obj_args, bma_map[bma_str], bma_args );
                cv::Mat reconst_img = cv::Mat_<uchar>( img_before.size(), 0 );

                for ( auto& [tlx, brx, tly, bry, vec_x, vex_y, error] :
                      motion_vec )
                {
                    cv::Mat reconst_Patch =
                        img_before( cv::Range( tlx + vec_x, brx + vec_x ),
                                    cv::Range( tly + vex_y, bry + vex_y ) );
                    reconst_Patch.copyTo( reconst_img(
                        cv::Range( tlx, brx ), cv::Range( tly, bry ) ) );
                }
                orig_forman.write( reconst_img );
                img_before = reconst_img;
            }
            std::cout << img_path.path().string() << " process done." << std::endl;
            nCount++;
        }
    }

    return 0;
}