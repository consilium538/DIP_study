#include "metric.hpp"

double
mad_patch( const cv::Mat ref,
           const cv::Mat comp,
           const double dx,
           const double dy,
           const std::vector<double>& args )
{
    return cv::sum( cv::abs( ref - comp ) )[0];
}

double
mse_patch( const cv::Mat ref,
           const cv::Mat comp,
           const double dx,
           const double dy,
           const std::vector<double>& args )
{
    return cv::sum( ( ref - comp ) * ( ref - comp ) )[0];
}

double
mad_dist( const cv::Mat ref,
          const cv::Mat comp,
          const double dx,
          const double dy,
          const std::vector<double>& args )
{
    return cv::sum( cv::abs( ref - comp ) )[0] +
           ( dx * dx + dy * dy ) * args[0];
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