#include "main.hpp"

int main( int argv, char** argc )
{
    ////////////////////////////////////

    namespace fs = std::filesystem;

    auto inputPath = fs::path( "./srcImg/" );
    auto savepath = fs::path( "./dstImg/" );
    auto logpath = fs::path( "./log" );

    if ( !fs::exists( savepath ) )
        fs::create_directory( savepath );
    if ( !fs::exists( logpath ) )
        fs::create_directory( logpath );

    auto bench_out_file = logpath / "bench.txt";
    auto valid_out_file = logpath / "valid.txt";

    std::streambuf* log_buf;
    std::ofstream log_fs;
    if ( isLogFile )
    {
        log_fs.open( valid_out_file.string(), std::ios::app );
        log_buf = log_fs.rdbuf();
    }
    else
    {
        log_buf = std::cout.rdbuf();
    }
    std::ostream valid_out( log_buf );
    std::ofstream bench_out;
    bench_out.open( bench_out_file.string(), std::ios::app );

    ////////////////////////////////////

    std::vector<std::tuple<cv::Mat, string>> ImgArr;
    std::unordered_map<std::string, cv::Mat> srcImg;
    std::map<std::string, cv::Mat> test_img;

    std::vector<std::string> testcase = {
        "checkerboard512", "crocodile", "footprint", "haze",    "moonlight",
        "pollen",          "polygon",   "symbol",    "white512"};  // latterB
    // std::vector<std::string> testcase = {"footprint", "pollen"};

    std::string header_pattern = "";
    //    "{0}_grass_4\t{0}_grass_8\t{0}_ctl_4\t{0}_ctl_8"
    //    "\t{0}_eq_set_4\t{0}_eq_set_8\t";
    std::vector label_method = {
        std::make_tuple( grassfire_4, "4_grassfire" ),
        std::make_tuple( grassfire_8, "8_grassfire" ),
        std::make_tuple( contour_tarck_4, "4_ctl" ),
        std::make_tuple( contour_tarck_8, "8_ctl" ),
        std::make_tuple( eq_label_set_4, "4_eq_label_set" ),
        std::make_tuple( eq_label_set_8, "8_eq_label_set" )};
    for ( auto it : label_method )
        header_pattern += fmt::format("{{0}}_{}\t",std::get<1>(it));

    ////////////////////////////////////

    auto cpu_string = CPUID_string();
    auto start_time = std::time( nullptr );
    bench_out << fmt::format( "\n{}\nProcess start at : {:%c}\n", cpu_string,
                              *std::localtime( &start_time ) )
              << std::endl;
    valid_out << fmt::format( "\n{}\nProcess start at : {:%c}\n", cpu_string,
                              *std::localtime( &start_time ) )
              << std::endl;

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
        ImgArr.push_back(
            std::make_tuple( img_tmp, fmt::format( "{}_{}", it, "orig" ) ) );
    }

    for ( auto it : test_img )
        bench_out << fmt::format( header_pattern, std::get<0>( it ) );

    valid_out << "file\\algo\t" << fmt::format( header_pattern, "N" )
              << std::endl;
    bench_out << std::endl;

    ////////////////////////////////////////

    for ( int i = 0; i < 41; i++ )
    {
        for ( auto it : test_img )
        {
            if ( i == 0 )
                valid_out << fmt::format( "{}\t", std::get<0>( it ) );
            for ( auto method : label_method )
            {
                cv::Mat img_test;
                cv::Mat img_ref = std::get<1>( it );
                int img_test_n;
                auto starttime = std::chrono::high_resolution_clock::now();
                std::tie( img_test, img_test_n ) =
                    std::get<0>( method )( std::get<1>( it ) );
                auto endtime = std::chrono::high_resolution_clock::now();
                double time_taken_ms =
                    (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                                  endtime - starttime )
                                  .count() ) *
                    1e-6;
                auto img_colored = colormap_custom( img_test );
                if ( i == 0 )
                {
                    valid_out << fmt::format( "{}\t", img_test_n );
                    ImgArr.push_back( std::make_tuple(
                        img_colored, fmt::format( "{}_{}", std::get<0>( it ),
                                                  std::get<1>( method ) ) ) );
                }
                else
                    bench_out << fmt::format( "{:.6g}\t", time_taken_ms );
            }
            if ( i == 0 )
                valid_out << "\n";
        }
        if ( i != 0 )
            bench_out << "\n";
    }

    valid_out << "////////////////" << std::endl;
    bench_out << "////////////////" << std::endl;

    img_save( ImgArr, savepath.string(), ".png",
              std::vector<int>{ cv::IMWRITE_PNG_COMPRESSION, 9 } );

    ////////////////////////////////////

    return 0;
}