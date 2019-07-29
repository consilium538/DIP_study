#include "metric.hpp"

double mad_patch( cv::Mat ref, cv::Mat comp )
{
    return cv::mean(cv::abs(ref-comp))[0];
}

double mse_patch( cv::Mat ref, cv::Mat comp )
{
    return cv::mean((ref-comp)*(ref-comp))[0];
}