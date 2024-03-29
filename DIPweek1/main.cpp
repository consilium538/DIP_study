#define _USE_MATH_DEFINES

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "affine.hpp"
#include "basic.hpp"
#include "color.hpp"
#include "conv.hpp"
#include "globals.hpp"
#include "intensity.hpp"

using namespace std;
using namespace cv;

int main( int argv, char** argc )
{
    // const string inputPath =
    //    "C:\\KDH\\19ysummer\\dip\\DIP4E Book Images Global Edition\\";
    // const string arrowFile = "salt_pepp.tif";
    // const string savepath = "C:\\KDH\\19ysummer\\dip\\week1\\";

    // Mat originalImg = cv::imread( inputPath + arrowFile );
    // Mat TransformedImg, MiddleImg;
    // Mat MiddleImgArr[3];

    // cv::split( originalImg, MiddleImgArr );
    // MiddleImg = MiddleImgArr[0];

    // string inputPath;
    // getline(cin, inputPath);
    // inputPath.erase(
    //    remove(inputPath.begin(), inputPath.end(), '\"'),
    //    inputPath.end()
    //);

    // Mat originalImg = cv::imread(inputPath);
    // if (!originalImg.data)
    //{
    //    cout << "Cannot read Img" << endl;
    //    return -1;
    //}

    // cv::namedWindow( "Original", cv::WINDOW_AUTOSIZE );
    // cv::moveWindow( "Original", 20, 20 );
    // cv::imshow( "Original", originalImg );

    // cv::waitKey();
    // cv::destroyAllWindows();

    // Intensity Transform

    // double c, r;
    // cin >> c >> r;

    // TransformedImg = intensityTransform(
    //    originalImg, logtransform(1)
    //);

    // MiddleImg = intensityTransform( originalImg, logtransform( 1 ) );
    // double a;
    // cv::minMaxLoc( MiddleImg, NULL, &a );
    // TransformedImg = MiddleImg / a * 255;
    // cv::minMaxLoc( originalImg, NULL, &a );
    // MiddleImg = originalImg / a * 255;

    // TransformedImg = intensityTransform(
    //    originalImg, thresholding(127)
    //    originalImg, levelslicing(180, 255, 230)
    //    originalImg, bitslicing(4)
    //);

    // Affine Transform

    // MiddleImg = ResizeTransform<Vec3b>( originalImg, 1.0 / 2.0, 1.0 / 2.0,
    //                                    InterpolationMethod::nearest );
    // TransformedImg = ResizeTransform<Vec3b>( MiddleImg, 2.0, 2.0,
    //                                         InterpolationMethod::nearest );

    // originalImg.convertTo(MiddleImg,CV_8UC1);

    // TransformedImg = GeometricTransform<Vec3b>(
    //    originalImg,
    //    translationMat( originalImg.rows / 2, originalImg.cols / 2 ) *
    //        rotateMat( 30.0 * 2 * M_PI / 360.0 ) *
    //        translationMat( -originalImg.rows / 2, -originalImg.cols / 2 ),
    //    InterpolationMethod::nearest );  //

    // Color Transform

    // MiddleImg = ColorTransform( originalImg, BGR2HSI );
    // cv::split( MiddleImg, MiddleImgArr );
    // TransformedImg = ColorTransform( MiddleImg, HSI2BGR );

    // Spatial filter

    // Smoothing spatial filter

    // originalImg.convertTo( MiddleImg, CV_64F );
    // conv2d( MiddleImg, gaussian_filter(21, 3.5), Padding::replicate)
    //    .convertTo( TransformedImg, CV_8U );

    // Adaptive median filter

    // auto start = chrono::high_resolution_clock::now();
    // TransformedImg = adaptive_median( originalImg, 7 );
    // auto end = chrono::high_resolution_clock::now();

    // double time_taken =
    //    chrono::duration_cast<chrono::nanoseconds>( end - start ).count();

    // time_taken *= 1e-9;

    // cout << "Time taken by program is : " << fixed << time_taken
    //     << setprecision( 9 );
    // cout << " sec" << endl;

    // print result

    // cv::namedWindow( "MiddleImg", cv::WINDOW_AUTOSIZE );
    // cv::moveWindow( "MiddleImg", 500, 20 );
    // cv::imshow( "MiddleImg", MiddleImg );

    const string inputPath =
        "C:\\KDH\\19ysummer\\dip\\DIP4E Book Images Global Edition\\";
    const string arrowFile = "moon-blurred.tif";
    const string savepath = "C:\\KDH\\19ysummer\\dip\\week1\\";

    Mat originalImg = cv::imread( inputPath + arrowFile );
    Mat TransformedImg, MiddleImg, AbcImg;
    Mat MiddleImgArr[3];

    cv::namedWindow( "Original", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "Original", 20, 20 );
    cv::imshow( "Original", originalImg );

    // todo
    originalImg.convertTo( MiddleImg, CV_64F );
    AbcImg = conv2d( MiddleImg, laplacian_filter(LaplaceOption::eight), Padding::replicate );

    cv::namedWindow( "Transformed", cv::WINDOW_AUTOSIZE );
    cv::moveWindow( "Transformed", 600, 20 );
    cv::imshow( "Transformed", TransformedImg );

    cv::waitKey( 3000 );
    cv::destroyAllWindows();

    cv::imwrite( savepath + "moon-blurred.png", originalImg,
                 std::vector<int>{cv::IMWRITE_PNG_COMPRESSION, 9} );

    cv::imwrite( savepath + "moon-blurred_laplace_.png", TransformedImg,
                 std::vector<int>{cv::IMWRITE_PNG_COMPRESSION, 9} );

    cv::imwrite( savepath + "moon-blurred.tif", TransformedImg );

    return 0;
}

// color transform check

// Mat input = Mat(256, 256, CV_8UC3);
// Mat log = Mat(256, 256, CV_8UC3);
// Mat ret = Mat(256, 256, CV_8UC3);
// Mat diff = Mat(256, 256, CV_8UC3);
// Mat split[3], h, s, l;
//
// for (int i = 0; i < 256; i++)
//{
//    for (int j = 0; j < 256; j++)
//    {
//        input.at<Vec3b>(i, j) = Vec3b(i, j, 127);
//        log.at<Vec3b>(i, j) = BGR2HSI(input.at<Vec3b>(i, j));
//        ret.at<Vec3b>(i, j) = HSI2BGR(log.at<Vec3b>(i, j));
//        diff.at<Vec3b>(i, j)[0] =
//            std::abs(ret.at<Vec3b>(i, j)[0] - input.at<Vec3b>(i, j)[0]);
//        diff.at<Vec3b>(i, j)[1] =
//            std::abs(ret.at<Vec3b>(i, j)[1] - input.at<Vec3b>(i, j)[1]);
//        diff.at<Vec3b>(i, j)[2] =
//            std::abs(ret.at<Vec3b>(i, j)[2] - input.at<Vec3b>(i, j)[2]);
//    }
//}
//
// cv::split(log, split);
// h = split[0]; s = split[1]; l = split[2];
//
// cv::imwrite( savepath + "strawberries-RGB_H.png",
//             MiddleImgArr[0],
//             std::vector<int>{cv::IMWRITE_PNG_COMPRESSION, 9} );
//
// cv::imwrite( savepath + "strawberries-RGB_S.png",
//             MiddleImgArr[1],
//             std::vector<int>{cv::IMWRITE_PNG_COMPRESSION, 9} );
//
// cv::imwrite( savepath + "strawberries-RGB_I.png",
//             MiddleImgArr[2],
//             std::vector<int>{cv::IMWRITE_PNG_COMPRESSION, 9} );