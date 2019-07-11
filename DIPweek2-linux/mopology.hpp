#ifndef MOPOLOGY_HPP
#define MOPOLOGY_HPP

#include "globals.hpp"

cv::Mat rectSE( const int size );
cv::Mat rectSE( const int x, const int y );
cv::Mat crossSE();

// A (-) B
cv::Mat erosion( cv::Mat A, cv::Mat B );

// A (+) B
cv::Mat dilation( cv::Mat A, cv::Mat B );

// A () B
cv::Mat opening( cv::Mat A, cv::Mat B );

// A (//) B
cv::Mat closing( cv::Mat A, cv::Mat B );

// D^(1)_G (F)
cv::Mat geodesic_dilation_1( cv::Mat F, cv::Mat G, cv::Mat B );

// D^(n)_G (F)
cv::Mat geodesic_dilation( cv::Mat F, cv::Mat G, cv::Mat B, int n );

// E^(1)_G (F)
cv::Mat geodesic_erosion_1( cv::Mat F, cv::Mat G, cv::Mat B );

// R^(D)_G (F)
cv::Mat geodesic_reconst_D( cv::Mat F, cv::Mat G, cv::Mat B );

// R^(E)_G (F)
cv::Mat geodesic_reconst_E( cv::Mat F, cv::Mat G, cv::Mat B );

// Hole of I
cv::Mat auto_hole( cv::Mat I );

// border cleaned image
cv::Mat border_clean( cv::Mat I );

#endif  // MOPOLOGY_HPP
