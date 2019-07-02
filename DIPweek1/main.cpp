#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "intensity.hpp"
#include "affine.hpp"

using namespace std;
using namespace cv;

int main(int argv, char** argc)
{
    const string inputPath = "C:\\KDH\\19ysummer\\dip\\DIP4E Book Images Global Edition\\";
    const string arrowFile = "angiogram-aortic-kidney.tif";
    Mat originalImg = cv::imread(inputPath + arrowFile);

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

    //cv::Mat TransformedImg = intensityTransform(
    //    originalImg, gammatransform(c, r)
    //);

    // Affine Transform

    //cv::Mat affine = AffineMatrix(1,0,0,0,1,0);

    //cv::Mat TransformedImg = GeometricTransform(
    //    originalImg, affine
    //);

    //cv::Mat TransformedImg = ResizeTransform(
    //    originalImg, 1.0/8.0, 1.0/8.0, 1
    //);
    cv::Mat TransformedImg = intensityTransform(
//        originalImg, thresholding(127)
//        originalImg, levelslicing(180, 255, 230)
//        originalImg, bitslicing(4)
    );

    //cv::Mat TransformedImg = ((cv::Mat_<double>(3, 3) <<
    //    1, 0, 0,
    //    0, 1, 0,
    //    0, 0, 1));

    cv::namedWindow("Transformed", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Transformed", 600, 20);
    cv::imshow("Transformed", TransformedImg);

    cv::waitKey();
    cv::destroyAllWindows();

	return 0;
}
