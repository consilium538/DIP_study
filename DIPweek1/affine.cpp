#include "affine.hpp"

cv::Mat AffineMat(
    double a11, double a12, double a13,
    double a21, double a22, double a23
)
{
    double coeff[] = { a11, a12, a13, a21, a22, a23, 0.0, 0.0, 1.0 };
    cv::Mat affine = cv::Mat(3, 3, CV_64F, coeff);
    return affine.clone();
}

cv::Mat rotateMat(double theta) {
    return (cv::Mat_<double>(3, 3) <<
        std::cos(theta), -std::sin(theta), 0,
        std::sin(theta), std::cos(theta), 0,
        0, 0, 1);
}

cv::Mat translationMat(double tx, double ty) {
    return (cv::Mat_<double>(3, 3) <<
        1, 0, tx,
        0, 1, ty,
        0, 0, 1);
}

std::tuple<double, double> AffineTransform(
    cv::Mat matrix, double x, double y
)
{
    double* ptr = matrix.ptr<double>();
    return std::make_tuple(
        x * ptr[0] + y * ptr[1] + ptr[2],
        x * ptr[3] + y * ptr[4] + ptr[5]
    );
}

cv::Mat GeometricTransform(
    cv::Mat sourceImg,
    cv::Mat affine
)
{
    // CV_Assert(sourceImg.type() == CV_8U);

    cv::Mat TransformedImg = cv::Mat::zeros(sourceImg.size(), sourceImg.type());
    const int nRows = TransformedImg.rows;
    const int nCols = TransformedImg.cols;
    uchar* pTransformed;
    double k, q;
    cv::Mat inverseAffine = affine.inv();

    //if(sourceImg.type() == CV_8U) {  }
    for (int i = 0; i < nRows; i++)
    {
        pTransformed = TransformedImg.ptr(i);
        for (int j = 0; j < nCols; j++)
        {
            std::tie(k, q) = AffineTransform(inverseAffine, i, j);
            // pTransformed[j] = ;
        }
    }

    return TransformedImg.clone();
}

cv::Mat ResizeTransform(
    cv::Mat sourceImg,
    double scaleX,
    double scaleY,
    int mode
)
{
    const int nRows = scaleX * sourceImg.rows;
    const int nCols = scaleY * sourceImg.cols;
    double inversescaleX = ((double)sourceImg.rows - 1.0) / (scaleX * (double)sourceImg.rows - 1.0);
    double inversescaleY = ((double)sourceImg.cols - 1.0) / (scaleY * (double)sourceImg.cols - 1.0);

    cv::Mat TransformedImg = cv::Mat(nRows, nCols, sourceImg.type());
    cv::Mat inverseAffine = (cv::Mat_<double>(3, 3) <<
        inversescaleX, 0, 0,
        0, inversescaleY, 0,
        0, 0, 1);

    uchar* pTransformed;
    double k, q;
    int ik, iq;

    switch (mode)
    {
    case 1: // nearleast neighberhood
        for (int i = 0; i < nRows; i++)
        {
            pTransformed = TransformedImg.ptr<uchar>(i);
            for (int j = 0; j < nCols; j++)
            {
                std::tie(k, q) = AffineTransform(inverseAffine, i, j);
                if (k >= 0.0 && k < sourceImg.rows &&
                    q >= 0.0 && q < sourceImg.cols)
                {
                    ik = (int)std::round(k);
                    iq = (int)std::round(q);

                    if (sourceImg.type() == CV_8U)
                    {
                        pTransformed[j] = sourceImg.at<unsigned char>(ik, iq);
                    }
                    else if (sourceImg.type() == CV_8UC3)
                    {
                        pTransformed[3 * j] = sourceImg.at<cv::Vec3b>(ik, iq)[0];
                        pTransformed[3 * j + 1] = sourceImg.at<cv::Vec3b>(ik, iq)[1];
                        pTransformed[3 * j + 2] = sourceImg.at<cv::Vec3b>(ik, iq)[2];
                    }
                }

                else // out off image
                    pTransformed[j] = 0; // zero padding
            }
        }
        break;
    default:
        break;
    }

    return TransformedImg.clone();
}