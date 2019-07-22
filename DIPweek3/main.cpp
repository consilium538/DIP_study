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

    catCPUID( log_out );
    auto start_time = std::time( nullptr );
    log_out << "Process start at " << std::ctime( &start_time ) << std::endl;

////////////////////////////////////////
// crocodile footprint haze moonlight pollen polygon symbol
    auto crocodile_orig = cv::imread( inputPath + "crocodile.bmp", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( crocodile_orig, "crocodile_orig" ) );

    if ( crocodile_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    cv::Mat crocodile_color = cv::Mat_<cv::Vec3b>(crocodile_orig.size(),cv::Vec3b(0,0,0));
    int k = 0;

    for( int i = 0; i < crocodile_orig.rows; i++ )
    {
        for ( int j = 0; j < crocodile_orig.cols; j++ )
        {
            if ( crocodile_orig.at<uchar>(i,j) != 0 )
            {
                k++;
                crocodile_color.at<cv::Vec3b>(i,j) = circularcolor(k);
            }
        }
    }
    ImgArr.push_back( std::make_tuple( crocodile_color, "crocodile_color" ) );
    img_cat(ImgArr);

////////////////////////////////////////

    log_out << "/////////////////////////////\n" << std::endl;

    // img_save(ImgArr, savepath.string(), ".png");

    return 0;
}
