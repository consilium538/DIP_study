#include "main.hpp"

int
main()
{
    namespace fs = std::filesystem;

    cv::VideoWriter orig_forman;
    cv::Mat sample_forman = cv::imread(
        "/home/dhej1/Downloads/Telegram "
        "Desktop/2014_DIP/FOREMAN(300)/FOREMAN000.tif" );
    orig_forman.open( "./forman_orig.mp4",
                      (int)cv::VideoWriter::fourcc( 'a', 'v', 'c', '1' ), 30.0,
                      sample_forman.size(), false );

    auto img_iter = fs::directory_iterator(
        "/home/dhej1/Downloads/Telegram Desktop/2014_DIP/FOREMAN(300)" );
    std::vector img_vec( begin( img_iter ), end( img_iter ) );
    std::sort( img_vec.begin(), img_vec.end() );

    for ( auto& it : img_vec )
    {
        cv::Mat img = cv::imread( it.path().string(), cv::IMREAD_GRAYSCALE );
        // std::cout << img.channels() << std::endl;
        orig_forman.write( img );
    }

    return 0;
}