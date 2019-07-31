#include "main.hpp"

int
main( int argv, char** argc )
{
    std::random_device rd;
    std::mt19937 e1(rd());
    std::uniform_int_distribution<int> unif_dist(0,255);

    cv::VideoWriter video_test;
    cv::Mat framebuf = cv::Mat_<cv::Vec3b>(cv::Size(240,320),0);

    video_test.open("./test.mp4",(int)cv::VideoWriter::fourcc('H','2','6','4'),30.0,cv::Size(240,320));

    for ( int i = 0; i < 150; i++)
    {
        framebuf(cv::Range(60,180),cv::Range(80,240)) = unif_dist(e1);
        video_test.write(framebuf);
    }

    ////////////////////////////////////

    return 0;
}
