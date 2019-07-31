#include "main.hpp"

int
main()
{
    namespace fs = std::filesystem;

    fs::path test_meta_path = "./log/meta.json";
    if ( !fs::exists( test_meta_path ) )
    {
        std::cout << "motion vector data not found!" << std::endl;
        return -1;
    }
    std::ifstream test_meta_in( test_meta_path.string() );
    Json::Value log_root;
    test_meta_in >> log_root;

    for ( auto test : log_root["tests"] )
    {
        // std::cout << fmt::format(
        //                  "anchor loc : {}\ntracked loc : {}\nmethod : {}",
        //                  test["file_loc"]["anchor_img_path"].asString(),
        //                  test["file_loc"]["tracked_img_path"].asString(),
        //                  test["method"].asString() )
        //           << "\n/////////////"
        //           << std::endl;

        cv::Mat anchor_img =
            cv::imread( test["file_loc"]["anchor_img_path"].asString() );
        cv::Mat tracked_img =
            cv::imread( test["file_loc"]["tracked_img_path"].asString() );
        std::vector<std::tuple<double,double,double,double,double>>
            motion_vec;
        
    }

    return 0;
}