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
    auto logfile = logpath / "log.txt";

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
    std::vector<std::string> testcase = {"crocodile", "footprint", "haze",
                                         "moonlight", "pollen",    "polygon",
                                         "symbol"};

    catCPUID( log_out );
    auto start_time = std::time( nullptr );
    log_out << "Process start at " << std::ctime( &start_time ) << std::endl;

    ////////////////////////////////////////

    log_out << "Test for Label-propagation(grassfire)" << std::endl;
    for ( auto it : testcase )
    {
        auto img_orig =
            cv::imread( inputPath + it + ".bmp", cv::IMREAD_GRAYSCALE );
        ImgArr.push_back( std::make_tuple( img_orig, it + "_orig" ) );
        if ( img_orig.empty() )
        {
            cout << "image load failed!" << endl;
            return -1;
        }

        cv::Mat img_4, img_8;
        int img_4_n, img_8_n;
        auto starttime = std::chrono::high_resolution_clock::now();
        std::tie( img_4, img_4_n ) = grassfire_4( img_orig );
        auto enttime_4 = std::chrono::high_resolution_clock::now();
        std::tie( img_8, img_8_n ) = grassfire_8( img_orig );
        auto enttime_8 = std::chrono::high_resolution_clock::now();
        double time_taken_4 =
            (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                          enttime_4 - starttime )
                          .count() ) *
            1e-6;
        double time_taken_8 =
            (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                          enttime_8 - enttime_4 )
                          .count() ) *
            1e-6;

        log_out << it << " process done!\n";
        log_out << "4-connectivity component : " << setw( 5 ) << img_4_n
                << " | spend time : " << fixed << time_taken_4
                << setprecision( 9 ) << "ms\n";
        log_out << "8-connectivity component : " << setw( 5 ) << img_8_n
                << " | spend time : " << fixed << time_taken_8
                << setprecision( 9 ) << "ms\n";
        log_out << "//-------------------------//" << std::endl;

        cv::Mat img_4_color = cv::Mat_<cv::Vec3b>( img_4.size() );
        img_4_color.forEach<cv::Vec3b>( [&]( cv::Vec3b& p, const int* i ) {
            int k = img_4.at<int>( i[0], i[1] );
            p = ( k == 0 ? 0 : circularcolor( k ) );
        } );
        ImgArr.push_back( std::make_tuple( img_4_color, it + "_4_grassfire" ) );

        cv::Mat img_8_color = cv::Mat_<cv::Vec3b>( img_8.size() );
        img_8_color.forEach<cv::Vec3b>( [&]( cv::Vec3b& p, const int* i ) {
            int k = img_8.at<int>( i[0], i[1] );
            p = ( k == 0 ? 0 : circularcolor( k ) );
        } );
        ImgArr.push_back( std::make_tuple( img_8_color, it + "_8_grassfire" ) );
    }

    ////////////////////////////////////////

    log_out << "Test for Label-equivalent-resolving(Equivalent-label-set)"
            << std::endl;
    for ( auto it : testcase )
    {
        auto img_orig =
            cv::imread( inputPath + it + ".bmp", cv::IMREAD_GRAYSCALE );
        ImgArr.push_back( std::make_tuple( img_orig, it + "_orig" ) );
        if ( img_orig.empty() )
        {
            cout << "image load failed!" << endl;
            return -1;
        }

        cv::Mat img_4, img_8;
        int img_4_n, img_8_n;
        auto starttime = std::chrono::high_resolution_clock::now();
        std::tie( img_4, img_4_n ) = eq_label_set_4( img_orig );
        auto enttime_4 = std::chrono::high_resolution_clock::now();
        std::tie( img_8, img_8_n ) = eq_label_set_8( img_orig );
        auto enttime_8 = std::chrono::high_resolution_clock::now();
        double time_taken_4 =
            (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                          enttime_4 - starttime )
                          .count() ) *
            1e-6;
        double time_taken_8 =
            (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                          enttime_8 - enttime_4 )
                          .count() ) *
            1e-6;

        log_out << it << " process done!\n";
        log_out << "4-connectivity component : " << setw( 5 ) << img_4_n
                << " | spend time : " << fixed << time_taken_4
                << setprecision( 9 ) << "ms\n";
        log_out << "8-connectivity component : " << setw( 5 ) << img_8_n
                << " | spend time : " << fixed << time_taken_8
                << setprecision( 9 ) << "ms\n";
        log_out << "//-------------------------//" << std::endl;

        cv::Mat img_4_color = cv::Mat_<cv::Vec3b>( img_4.size() );
        img_4_color.forEach<cv::Vec3b>( [&]( cv::Vec3b& p, const int* i ) {
            int k = img_4.at<int>( i[0], i[1] );
            p = ( k == 0 ? 0 : circularcolor( k ) );
        } );
        ImgArr.push_back(
            std::make_tuple( img_4_color, it + "_4_eq_label_set" ) );

        cv::Mat img_8_color = cv::Mat_<cv::Vec3b>( img_8.size() );
        img_8_color.forEach<cv::Vec3b>( [&]( cv::Vec3b& p, const int* i ) {
            int k = img_8.at<int>( i[0], i[1] );
            p = ( k == 0 ? 0 : circularcolor( k ) );
        } );
        ImgArr.push_back(
            std::make_tuple( img_8_color, it + "_8_eq_label_set" ) );
    }

    ////////////////////////////////////////

    log_out << "/////////////////////////////\n" << std::endl;

    // img_save(ImgArr, savepath.string(), ".png");

    return 0;
}