#include "main.hpp"

int main( int argv, char** argc )
{
    namespace fs = std::filesystem;

    auto inputPath = fs::path( "./srcImg/" );
    auto savepath = fs::path( "./dstImg/" );

    if ( !fs::exists( savepath ) )
        fs::create_directory( savepath );

    auto logpath = fs::path( "./log" );
    if ( !fs::exists( logpath ) )
        fs::create_directory( logpath );
    auto logfile = logpath / "bench.txt";
    auto valid_out_file = logpath / "valid.txt";

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
    std::ostream bench_out( log_buf );

    std::ofstream valid_out;
    valid_out.open( valid_out_file.string(), std::ios::app );

    std::vector<std::tuple<cv::Mat, string>> ImgArr;
    std::unordered_map<std::string, cv::Mat> srcImg;
    std::vector<std::string> testcase = {"crocodile", "footprint", "haze",
                                        "moonlight", "pollen", "polygon",
                                        "symbol",
                                        "checkerboard512","white512"};
    // std::vector<std::string> testcase = {"crocodile"};

    std::map<std::string, cv::Mat> test_img;

    auto cpu_string = CPUID_string();
    auto start_time = std::time( nullptr );
    bench_out << fmt::format( "\n{}\nProcess start at : {:%c}\n",
                            cpu_string, *std::localtime(&start_time) )
            << std::endl;
    valid_out << fmt::format( "\n{}\nProcess start at : {:%c}\n",
                              cpu_string, *std::localtime(&start_time) )
              << std::endl;

    const string header_pattern =
//        "{0}_grass_4\t{0}_grass_8\t{0}_eq_set_4\t{0}_eq_set_8\t";
        "{0}_grass_4\t{0}_grass_8\t";
    // std::vector<std::function<std::tuple<cv::Mat,int>(cv::Mat)>>
    // std::vector label_method = {
    //     grassfire_4, grassfire_8,
    //     eq_label_set_4, eq_label_set_8
    // };

    for ( auto it : testcase )
    {
        auto img_path = inputPath / ( it + ".bmp" );
        auto img_tmp = cv::imread( img_path.string(), cv::IMREAD_GRAYSCALE );
        if ( img_tmp.empty() )
        {
            cout << "image load failed!" << endl;
            bench_out << "\nimage load failed!\n" << std::endl;
            return -1;
        }
        test_img[it] = img_tmp;
    }

    for ( auto it : test_img )
        bench_out << fmt::format( header_pattern, std::get<0>( it ) );

    valid_out << "file_algo\t" << fmt::format( header_pattern, "N" )
              << std::endl;
    bench_out << std::endl;

    ////////////////////////////////////////

    for ( int i = 0; i < 2; i++ )
    {
        for ( auto it : test_img )
        {
            cv::Mat img_4_g, img_8_g;
            int img_4_n_g, img_8_n_g;
            auto starttime_g = std::chrono::high_resolution_clock::now();
            std::tie( img_4_g, img_4_n_g ) = grassfire_4( std::get<1>( it ) );
            // std::tie( img_4_g, img_4_n_g ) = grassfire_4( std::get<1>( it ) );
            auto enttime_4_g = std::chrono::high_resolution_clock::now();
            std::tie( img_8_g, img_8_n_g ) = grassfire_8( std::get<1>( it ) );
            // std::tie( img_8_g, img_8_n_g ) = grassfire_8( std::get<1>( it ) );
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
            std::tie( img_4_e, img_4_n_e ) =
                eq_label_set_4( std::get<1>( it ) );
            auto enttime_4_e = std::chrono::high_resolution_clock::now();
            std::tie( img_8_e, img_8_n_e ) =
                eq_label_set_8( std::get<1>( it ) );
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
            if ( i == 0 )
                valid_out << fmt::format( "{}\t{}\t{}\t{}\t{}\t\n",
                                          std::get<0>( it ), img_4_n_g,
                                          img_8_n_g, img_4_n_e, img_8_n_e );
            else
                bench_out << fmt::format( "{:.6g}\t{:.6g}\t{:.6g}\t{:.6g}\t",
                                        time_taken_4_g, time_taken_8_g,
                                        time_taken_4_e, time_taken_8_e );
        }
        if ( i != 0 )
            bench_out << "\n";
    }

    valid_out << "////////////////" << std::endl;
    bench_out << "////////////////" << std::endl;

    return 0;
}