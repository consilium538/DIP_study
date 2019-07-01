#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "intensity.hpp"

using namespace std;
using namespace cv;

int main(int argv, char** argc)
{
    string inputPath;
    getline(cin, inputPath);
    inputPath.erase(
        remove(inputPath.begin(), inputPath.end(), '\"'),
        inputPath.end()
    );

    Mat originalImg = cv::imread(inputPath);
    if (!originalImg.data)
    {
        cout << "Cannot read Img" << endl;
        return -1;
    }

    //double c, r;
    //cin >> c >> r;

    //cv::Mat TransformedImg = intensityTransform(
    //    originalImg, gammatransform(c, r)
    //);
 
    cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("Transformed", cv::WINDOW_AUTOSIZE);

    cv::moveWindow("Original", 20, 20);
    //cv::moveWindow("Transformed", 600, 20);

    cv::imshow("Original", originalImg);
    //cv::imshow("Transformed", TransformedImg);

    cv::waitKey();
    cv::destroyAllWindows();

    // Affine Transform

    Mat TransformedImg = Mat::zeros(originalImg.size(),originalImg.type());
    int nRows = TransformedImg.rows;
    int nCols = TransformedImg.cols;
    uchar* p;

    for (int i = 0; i < nRows; i++)
    {
        p = TransformedImg.ptr(i);
        for (int j = 0; j < nCols; j++)
        {
            p[j] = (p[j]);
        }
    }

	return 0;
}