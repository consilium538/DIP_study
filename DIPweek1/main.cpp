#define _USE_MATH_DEFINES

#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "intensity.hpp"
#include "affine.hpp"

using namespace std;
using namespace cv;

int main(int argv, char** argc)
{
    const string inputPath = "C:\\KDH\\19ysummer\\dip\\DIP4E Book Images Global Edition\\";
    const string arrowFile = "wingding-arrow-right.tif";
    Mat originalImg = cv::imread(inputPath + arrowFile);
    Mat TransformedImg;

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
 
    //cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
    //cv::moveWindow("Original", 20, 20);
    //cv::imshow("Original", originalImg);

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

    // print result

    cv::namedWindow("Transformed", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Transformed", 600, 20);
    cv::imshow("Transformed", TransformedImg);

    cv::waitKey();
    cv::destroyAllWindows();

	return 0;
}
