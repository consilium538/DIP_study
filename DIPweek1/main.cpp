#define _USE_MATH_DEFINES

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "globals.hpp"
#include "intensity.hpp"
#include "affine.hpp"
#include "color.hpp"
#include "basic.hpp"
#include "conv.hpp"

using namespace std;
using namespace cv;

int main(int argv, char** argc)
{
    const string inputPath = "C:\\KDH\\19ysummer\\dip\\DIP4E Book Images Global Edition\\";
    const string arrowFile = "testpattern512.tif";
    Mat originalImg = cv::imread(inputPath + arrowFile);
    Mat TransformedImg, MiddleImg;

    //string inputPath;
    //getline(cin, inputPath);
    //inputPath.erase(
    //    remove(inputPath.begin(), inputPath.end(), '\"'),
    //    inputPath.end()
    //);

    //Mat originalImg = cv::imread(inputPath);
    //if (!originalImg.data)
    //{
    //    cout << "Cannot read Img" << endl;
    //    return -1;
    //}
 
    cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Original", 20, 20);
    cv::imshow("Original", originalImg);

    //cv::waitKey();
    //cv::destroyAllWindows();

    // Intensity Transform

    //double c, r;
    //cin >> c >> r;

    //TransformedImg = intensityTransform(
    //    originalImg, gammatransform(c, r)
    //);

    //TransformedImg = intensityTransform(
    //    originalImg, thresholding(127)
    //    originalImg, levelslicing(180, 255, 230)
    //    originalImg, bitslicing(4)
    //);

    // Affine Transform

    //TransformedImg = ResizeTransform(
    //    originalImg, 1.0/8.0, 1.0/8.0, 1
    //);

    //TransformedImg = GeometricTransform(
    //    originalImg,
    //    translationMat(originalImg.rows/2, originalImg.cols/2) *
    //    rotateMat(30.0 * 2 * M_PI / 360.0) *
    //    translationMat(-originalImg.rows / 2, -originalImg.cols / 2)
    //); // 

    // Color Transform

    //MiddleImg = ColorTransform(originalImg, BGR2HSI);
    //TransformedImg = ColorTransform(MiddleImg, HSI2BGR);
    
    // Spatial filter

    originalImg.convertTo(MiddleImg, CV_64F);
    conv2d(
        MiddleImg,
        (cv::Mat_<double>(3,3) <<
            1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
            1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 
            1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0),
        1).convertTo(TransformedImg, CV_8U);

    // Smoothing spatial filter

    // print result

    cv::namedWindow("Transformed(BGR->HSB->BGR)", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Transformed(BGR->HSB->BGR)", 600, 20);
    cv::imshow("Transformed(BGR->HSB->BGR)", TransformedImg);

    cv::waitKey();
    cv::destroyAllWindows();



	return 0;
}

// color transform check

//Mat input = Mat(256, 256, CV_8UC3);
//Mat log = Mat(256, 256, CV_8UC3);
//Mat ret = Mat(256, 256, CV_8UC3);
//Mat diff = Mat(256, 256, CV_8UC3);
//Mat split[3], h, s, l;
//
//for (int i = 0; i < 256; i++)
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
//cv::split(log, split);
//h = split[0]; s = split[1]; l = split[2];