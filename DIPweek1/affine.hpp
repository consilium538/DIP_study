#include <opencv2/core/utility.hpp>

cv::Mat AffineMat(
    double a11, double a12, double a13,
    double a21, double a22, double a23
)
{
    double coeff[] = { a11, a12, a13, a21, a22, a23, 0.0, 0.0, 1.0 };
    cv::Mat affine = cv::Mat(3, 3, CV_64F, coeff);
    return affine.clone();
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
