#include "conv.hpp"

cv::Mat conv2d(cv::Mat large, cv::Mat small, int method)
{
    //CV_Assert(large.type() == CV_64F);
    cv::Mat conv;
    conv = cv::Mat(large.size(), CV_64FC3);
    int nRow = large.rows;
    int nCol = large.cols;
    int nRowS = small.rows;
    int nColS = small.cols;
    cv::Vec3d* ptr;
    cv::Mat focusMat = cv::Mat(small.size(), CV_64FC3);

    // int nRow = method == 1 ? large.rows : large.rows + small.rows;

    switch (method)
    {
    case 1: // zero padding
        for (int i = 0; i < nRow; i++)
        {
            ptr = conv.ptr<cv::Vec3d>(i);
            for (int j = 0; j < nCol; j++)
            {
                for (int k = 0; k < nRowS; k++)
                {
                    int k_centered;

                    k_centered = i + k - nRowS / 2;
                    for (int l = 0; l < nColS; l++)
                    {
                        int l_centered;

                        l_centered = j + l - nColS / 2;
                        // if k_centered lower bound over
                        if (k_centered < 0)
                            focusMat.at<cv::Vec3d>(k, l) = { 0.0,0.0,0.0 };
                        // if k_centered upper bound over
                        else if (k_centered >= nRow)
                            focusMat.at<cv::Vec3d>(k, l) = { 0.0,0.0,0.0 };
                        // if l_centered lower bound over
                        else if (l_centered < 0)
                            focusMat.at<cv::Vec3d>(k, l) = { 0.0,0.0,0.0 };
                        // if l_centered upper bound over
                        else if (l_centered >= nRow)
                            focusMat.at<cv::Vec3d>(k, l) = { 0.0,0.0,0.0 };
                        else
                            focusMat.at<cv::Vec3d>(k, l)
                              = large.at<cv::Vec3d>(k_centered, l_centered);
                    }
                }
                ptr[j] = cv::sum(focusMat.mul(small));
            }
        }
    }

    return conv.clone();
}
