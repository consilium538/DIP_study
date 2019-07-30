#include "metric.hpp"

double
mad_patch( cv::Mat ref,
           cv::Mat comp,
           double dx,
           double dy,
           std::vector<double>& args )
{
    return cv::mean( cv::abs( ref - comp ) )[0];
}

double
mse_patch( cv::Mat ref,
           cv::Mat comp,
           const double dx,
           const double dy,
           std::vector<double>& args )
{
    return cv::mean( ( ref - comp ) * ( ref - comp ) )[0];
}

// std::optional<double>
// metric_eval( cv::Mat ref,
//              cv::Mat comp,
//              const int x,
//              const int y,
//              const double dx,
//              const double dy,
//              std::function<double( cv::Mat,
//                                    cv::Mat,
//                                    const double,
//                                    const double,
//                                    std::vector<double>& )> metric,
//              std::vector<double>& args )
// {
//     const int nRows = ref.rows;
//     const int nCols = ref.cols;
//     if ( x < 0 | y < 0 | x )
//         ;
// }