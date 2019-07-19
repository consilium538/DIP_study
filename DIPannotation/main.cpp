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

    auto now = std::time( nullptr );
    log_out << "Process start at " << std::ctime( &now ) << std::endl;

//    auto text_broken_orig =
//        cv::imread( inputPath + "text_broken_orig.png", cv::IMREAD_GRAYSCALE );
//
//    auto text_broken_dil =
//        cv::imread( inputPath + "text_broken_dil.png", cv::IMREAD_GRAYSCALE );
//
//    auto text_box = cv::Rect(cv::Point(867,690),cv::Point(935,726));
//    auto text_broken_orig_cut = text_broken_orig(text_box).clone();
//    ImgArr.push_back( std::make_tuple( text_broken_orig_cut, "text_broken_orig_cut" ) );
//    auto text_broken_dil_cut = text_broken_dil(text_box).clone();
//    ImgArr.push_back( std::make_tuple( text_broken_dil_cut, "text_broken_dil_cut" ) );
//    cv::rectangle(text_broken_orig,text_box,Scalar(127),3);
//    cv::rectangle(text_broken_dil,text_box,Scalar(127),3);
//    ImgArr.push_back( std::make_tuple( text_broken_orig, "text_broken_orig_boxed" ) );
//    ImgArr.push_back( std::make_tuple( text_broken_dil, "text_broken_dil_boxed" ) );
//
//    img_save(ImgArr,savepath.string(),".png");
    
    // 867, 690 ~ 930, 726
    auto ball_orig = cv::imread( inputPath + "ball_orig.png",
                                 cv::IMREAD_GRAYSCALE );

    if ( ball_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    std::vector<std::tuple<int, int>> hole_init = {
        {60, 50},   {180, 45},  {363, 39},  {93, 159},  {178, 203},
        {262, 148}, {414, 233}, {461, 142}, {104, 299}, {236, 308},
        {386, 371}, {489, 357}, {56, 451},  {235, 460}, {388, 371},
        {489, 358}, {58, 451},  {234, 459}, {505, 490}, {399, 471}};

    //auto ball_hole = cv::Mat( ball_orig.size(), CV_8UC1, cv::Scalar( 0 ) );
    auto ball_hole = ball_orig.clone();

    for ( auto it : hole_init )
        ball_hole.at<uchar>( std::get<1>( it ), std::get<0>( it ) ) = 255;
    ImgArr.push_back( std::make_tuple( ball_hole, "ball_hole" ) );

    img_save(ImgArr,savepath.string(),".png");

    return 0;
}
