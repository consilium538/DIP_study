#include "main.hpp"

int main( int argv, char** argc )
{
    namespace fs = std::filesystem;

    const string inputPath = "./srcImg/";
    auto savepath = fs::path( "./dstImg/" );
    if ( !fs::exists( savepath ) )
        fs::create_directory( savepath );

    auto logpath = fs::path( "./log" );
    if ( !fs::exists( logpath ) )
        fs::create_directory( logpath );
    auto logfile = logpath / "bench.txt";

    std::streambuf* log_buf;
    std::ofstream log_fs;
    if ( isLogFile )
    {
        log_fs.open( logfile.string(), std::ios::app );
        log_buf = log_fs.rdbuf();
    }
    else
    {
        log_buf = std::cout.rdbuf();
    }
    std::ostream log_out( log_buf );

    std::vector<std::tuple<cv::Mat, string>> ImgArr;
    std::unordered_map<std::string, cv::Mat> srcImg;
    //std::vector<std::string> testcase = {"crocodile", "footprint", "haze",
    //                                     "moonlight", "pollen",    "polygon",
    //                                     "symbol"};
    std::vector<std::string> testcase = {"checkerboard512","white512"};

    ////////////////////////////////////////

    for ( int i = 0; i < 40; i++ )
    {
        for ( auto it : testcase )
        {
            auto img_orig =
                cv::imread( inputPath + it + ".bmp", cv::IMREAD_GRAYSCALE );
            if ( img_orig.empty() )
            {
                cout << "image load failed!" << endl;
                return -1;
            }

            cv::Mat img_4_g, img_8_g;
            int img_4_n_g, img_8_n_g;
            auto starttime_g = std::chrono::high_resolution_clock::now();
            std::tie( img_4_g, img_4_n_g ) = grassfire_4( img_orig );
            auto enttime_4_g = std::chrono::high_resolution_clock::now();
            std::tie( img_8_g, img_8_n_g ) = grassfire_8( img_orig );
            auto enttime_8_g = std::chrono::high_resolution_clock::now();
            double time_taken_4_g =
                (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                              enttime_4_g - starttime_g )
                              .count() ) *
                1e-6;
            double time_taken_8_g =
                (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                              enttime_8_g - enttime_4_g )
                              .count() ) *
                1e-6;

            cv::Mat img_4_e, img_8_e;
            int img_4_n_e, img_8_n_e;
            auto starttime_e = std::chrono::high_resolution_clock::now();
            std::tie( img_4_e, img_4_n_e ) = eq_label_set_4( img_orig );
            auto enttime_4_e = std::chrono::high_resolution_clock::now();
            std::tie( img_8_e, img_8_n_e ) = eq_label_set_8( img_orig );
            auto enttime_8_e = std::chrono::high_resolution_clock::now();
            double time_taken_4_e =
                (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                              enttime_4_e - starttime_e )
                              .count() ) *
                1e-6;
            double time_taken_8_e =
                (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                              enttime_8_e - enttime_4_e )
                              .count() ) *
                1e-6;

            log_out << fixed << setprecision( 6 ) << time_taken_4_g << " ";
            log_out << fixed << setprecision( 6 ) << time_taken_8_g << " ";
            log_out << fixed << setprecision( 6 ) << time_taken_4_e << " ";
            log_out << fixed << setprecision( 6 ) << time_taken_8_e << " ";
        }
        log_out << std::endl;
    }

    return 0;
}