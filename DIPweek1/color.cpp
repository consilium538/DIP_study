#include "color.hpp"

cv::Mat ColorTransform(
    cv::Mat img,
    std::function<cv::Vec3b(cv::Vec3b)> func
)
{
    CV_Assert(img.type() == CV_8UC3);

    int channels = img.channels();
    int nRows = img.rows;
    int nCols = img.cols;

    int i, j;
    cv::Vec3b *p, *q;
    cv::Mat returnImg = cv::Mat::zeros(
        img.size(), img.type());

    if (img.isContinuous() && returnImg.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }
    for (i = 0; i < nRows; ++i)
    {
        p = img.ptr<cv::Vec3b>(i);
        q = returnImg.ptr<cv::Vec3b>(i);
        for (j = 0; j < nCols; ++j)
        {
            q[j] = func(p[j]);
        }
    }
    return returnImg.clone();
}

cv::Vec3b ColorIdentity(cv::Vec3b color)
{
    return color;
}

cv::Vec3b BGR2HSI(cv::Vec3b color)
{
    double theta = color[0] == color[1] && color[0] == color[2] ? 0.0 : 
        std::acos((2* (double)color[2]- (double)color[1]- (double)color[0]) / 2.0
        /std::sqrt( (color[2] - color[1]) * (color[2] - color[1])
            + (color[2] - color[0]) * (color[1] - color[0])));
    double H = color[0] <= color[1] ? theta / (2 * M_PI) : 1 - theta / (2 * M_PI);
    double S = (color[0] + color[1] + color[2]) == 0 ? 0.0 :
        1.0 - 3.0 * std::min(std::min(color[0], color[1]), color[2]) /
        ((double)color[0] + (double)color[1] + (double)color[2]);
    double I = ((double)color[0] + (double)color[1] + (double)color[2]) / 3.0;

    return cv::Vec3b(threshold(H * 256), threshold(S * 256), threshold(I));
}

cv::Vec3b HSI2BGR(cv::Vec3b color)
{
    double R, G, B;
    double rad_H;
    if (color[0] < 256.0 / 3) // RG sector
    {
        rad_H = (color[0] / 128.0) * M_PI;
        B = color[2] * (256.0 - color[1]) / 256.0;
        R = color[2] * (256.0 + color[1] * std::cos(rad_H) / std::cos(M_PI / 3.0 - rad_H)) / 256.0;
        G = 3 * (double)color[2] - (R + B);
    }
    else if (color[0] < 256.0 * 2 / 3) // GB sector
    {
        rad_H = (color[0] / 128.0 - 2.0/3.0) * M_PI;
        R = color[2] * (256.0 - color[1]) / 256.0;
        G = color[2] * (256.0 + color[1] * std::cos(rad_H) / std::cos(M_PI / 3.0 - rad_H)) / 256.0;
        B = 3 * (double)color[2] - (R + G);
    }
    else // BR sector
    {
        rad_H = (color[0] / 128.0 - 4.0 / 3.0) * M_PI;
        G = color[2] * (256.0 - color[1]) / 256.0;
        B = color[2] * (256.0 + color[1] * std::cos(rad_H) / std::cos(M_PI / 3.0 - rad_H)) / 256.0;
        R = 3 * (double)color[2] - (G + B);
    }
    return cv::Vec3b(threshold(B), threshold(G), threshold(R));
    //return color;
}
