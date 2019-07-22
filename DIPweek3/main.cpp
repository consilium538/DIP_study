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
    // crocodile footprint haze moonlight pollen polygon symbol
    auto crocodile_orig =
        cv::imread( inputPath + "crocodile.bmp", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( crocodile_orig, "crocodile_orig" ) );

    if ( crocodile_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    srcImg["crocodile"] = crocodile_orig;

    auto footprint_orig =
        cv::imread( inputPath + "footprint.bmp", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( footprint_orig, "footprint_orig" ) );

    if ( footprint_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    srcImg["footprint"] = footprint_orig;

    cv::Mat crocodile_4, crocodile_8, footpring_4, footpring_8;
    int crocodile_4_n, crocodile_8_n, footpring_4_n, footpring_8_n;

    auto starttime = std::chrono::high_resolution_clock::now();
    std::tie( crocodile_4, crocodile_4_n ) = grassfire_4( crocodile_orig );
    auto crocodile_4_endtime = std::chrono::high_resolution_clock::now();
    std::tie( crocodile_8, crocodile_8_n ) = grassfire_8( crocodile_orig );
    auto crocodile_8_endtime = std::chrono::high_resolution_clock::now();
    std::tie( footpring_4, footpring_4_n ) = grassfire_4( footprint_orig );
    auto footpring_4_endtime = std::chrono::high_resolution_clock::now();
    std::tie( footpring_8, footpring_8_n ) = grassfire_8( footprint_orig );
    auto footpring_8_endtime = std::chrono::high_resolution_clock::now();

    double time_taken_crocodile_4 =
        (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                      crocodile_4_endtime - starttime )
                      .count() ) *
        1e-9;
    double time_taken_crocodile_8 =
        (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                      crocodile_8_endtime - crocodile_4_endtime )
                      .count() ) *
        1e-9;
    double time_taken_footpring_4 =
        (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                      footpring_4_endtime - crocodile_8_endtime )
                      .count() ) *
        1e-9;
    double time_taken_footpring_8 =
        (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                      footpring_8_endtime - footpring_4_endtime )
                      .count() ) *
        1e-9;

    log_out << "spend time : \n";
    log_out << "crocodile 4-connectivity : " << fixed << time_taken_crocodile_4
            << setprecision( 9 ) << "\n";
    log_out << "crocodile 8-connectivity : " << fixed << time_taken_crocodile_8
            << setprecision( 9 ) << "\n";
    log_out << "footpring 4-connectivity : " << fixed << time_taken_footpring_4
            << setprecision( 9 ) << "\n";
    log_out << "footpring 8-connectivity : " << fixed << time_taken_footpring_8
            << setprecision( 9 ) << "\n";

    cv::Mat footpring_8_color = cv::Mat_<cv::Vec3b>( footpring_8.size() );
    footpring_8_color.forEach<cv::Vec3b>( [&]( cv::Vec3b& p, const int* i ) {
        int k = footpring_8.at<int>( i[0], i[1] );
        p = ( k == 0 ? 0 : circularcolor( k ) );
    } );

    ////////////////////////////////////////

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
            1e-9;
        double time_taken_8 =
            (double)( std::chrono::duration_cast<chrono::nanoseconds>(
                          enttime_8 - enttime_4 )
                          .count() ) *
            1e-9;

        log_out << it << " process done!\n";
        log_out << "4-connectivity component : " << setw( 5 ) << img_4_n
                << "spend time : " << fixed << time_taken_4
                << setprecision( 9 ) << "\n";
        log_out << "8-connectivity component : " << setw( 5 ) << img_8_n
                << "spend time : " << fixed << time_taken_8
                << setprecision( 9 ) << "\n";
        log_out << "//////\n" << std::endl;
    }

    ////////////////////////////////////////

    log_out << "/////////////////////////////\n" << std::endl;

    // img_save(ImgArr, savepath.string(), ".png");

    return 0;
}

//    cv::Mat crocodile_color =
//    cv::Mat_<cv::Vec3b>(crocodile_orig.size(),cv::Vec3b(0,0,0)); int k = 0;
//
//    for( int i = 0; i < crocodile_orig.rows; i++ )
//    {
//        for ( int j = 0; j < crocodile_orig.cols; j++ )
//        {
//            if ( crocodile_orig.at<uchar>(i,j) != 0 )
//            {
//                k++;
//                crocodile_color.at<cv::Vec3b>(i,j) = circularcolor(k);
//            }
//        }
//    }
//    ImgArr.push_back( std::make_tuple( crocodile_color, "crocodile_color" ) );