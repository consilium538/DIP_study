#include "mopology.hpp"

cv::Mat rectSE( const int size )
{
    return cv::Mat( size, size, CV_8UC1, cv::Scalar( 255 ) );
}

cv::Mat rectSE( const int x, const int y )
{
    return cv::Mat( x, y, CV_8UC1, cv::Scalar( 255 ) );
}

cv::Mat crossSE()
{
    return ( cv::Mat_<uchar>( 3, 3 ) << 0, 255, 0, 255, 255, 255, 0, 255, 0 );
}

std::function<void( Pixel&, const int* )> singleerosion( cv::Mat& A,
                                                         cv::Mat& B )
{
    return [&]( Pixel& p, const int* position ) -> void {
        for ( int k = -( B.rows / 2 ); k <= ( B.rows / 2 ); k++ )
            for ( int l = -( B.cols / 2 ); l <= ( B.cols / 2 ); l++ )
                if ( position[0] + k >= 0 && position[0] + k < A.rows &&
                     position[1] + l >= 0 && position[1] + l < A.cols &&
                     B.at<uchar>( ( B.rows / 2 ) - k, ( B.cols / 2 ) - l ) ==
                         255 &&
                     A.at<uchar>( position[0] + k, position[1] + l ) == 0 )
                {
                    p = 0;
                    return;
                }
        p = 255;
        return;
    };
}

// A (-) B
cv::Mat erosion( cv::Mat A, cv::Mat B )
{
    cv::Mat C = cv::Mat_<uchar>( A.size() );

    const int nRows = C.rows;
    const int nCols = C.cols;
    int p[2];

    auto f = singleerosion( A, B );

    for ( int i = 0; i < nRows; i++ )
        for ( int j = 0; j < nCols; j++ )
        {
            // C.at<uchar>( i, j ) = singleerosion( A, B, i, j );

            p[0] = i;
            p[1] = j;
            f( C.at<uchar>( i, j ), p );
        }

    return C;
}

std::function<void( Pixel&, const int* )> singledilation( cv::Mat& A,
                                                          cv::Mat& B )
{
    return [&]( Pixel& p, const int* position ) -> void {
        for ( int k = -( B.rows / 2 ); k <= ( B.rows / 2 ); k++ )
            for ( int l = -( B.cols / 2 ); l <= ( B.cols / 2 ); l++ )
                if ( position[0] + k >= 0 && position[0] + k < A.rows &&
                     position[1] + l >= 0 && position[1] + l < A.cols &&
                     B.at<uchar>( ( B.rows / 2 ) - k, ( B.cols / 2 ) - l ) ==
                         255 &&
                     A.at<uchar>( position[0] + k, position[1] + l ) == 255 )
                {
                    p = 255;
                    return;
                }
        p = 0;
        return;
    };
}

// A (+) B
cv::Mat dilation( cv::Mat A, cv::Mat B )
{
    cv::Mat C = cv::Mat_<uchar>( A.size() );

    const int nRows = C.rows;
    const int nCols = C.cols;

    C.forEach<Pixel>( singledilation( A, B ) );

    return C;
}

cv::Mat opening( cv::Mat A, cv::Mat B )
{
    return dilation( erosion( A, B ), B );
}

cv::Mat closing( cv::Mat A, cv::Mat B )
{
    return erosion( dilation( A, B ), B );
}

cv::Mat geodesic_dilation_1( cv::Mat F, cv::Mat G, cv::Mat B )
{
    auto dil = dilation( F, B );
    dil.forEach<uchar>( [&]( Pixel& p, const int* i ) {
        p = p && G.at<uchar>( i[0], i[1] ) ? 255 : 0;
    } );
    return dil;
}

cv::Mat geodesic_dilation( cv::Mat F, cv::Mat G, cv::Mat B, int n )
{
    for ( int i = 0; i < n; i++ )
        F = geodesic_dilation_1( F, G, B );

    return F;
}

cv::Mat geodesic_erosion_1( cv::Mat F, cv::Mat G, cv::Mat B )
{
    auto eros = erosion( F, B );
    eros.forEach<uchar>( [&]( Pixel& p, const int* i ) {
        p = p | G.at<uchar>( i[0], i[1] ) ? 255 : 0;
    } );
    return eros;
}

cv::Mat geodesic_reconst_D( cv::Mat F, cv::Mat G, cv::Mat B )
{
    cv::Mat D;
    bool isSame = true;
    for ( int i = 0; i < 1000; i++ )
    {
        D = geodesic_dilation_1( F, G, B );
        D.forEach<uchar>( [&]( Pixel& p, const int* i ) {
            if ( p != F.at<uchar>( i[0], i[1] ) )
                isSame = false;
        } );

        if ( isSame == true )
            break;

        isSame = true;
        F = D;
    }

    return D;
}

cv::Mat geodesic_reconst_E( cv::Mat F, cv::Mat G, cv::Mat B )
{
    cv::Mat E;
    bool isSame = true;
    for ( int i = 0; i < 1000; i++ )
    {
        E = geodesic_erosion_1( F, G, B );
        E.forEach<uchar>( [&]( Pixel& p, const int* i ) {
            if ( p != F.at<uchar>( i[0], i[1] ) )
                isSame = false;
        } );

        if ( isSame == true )
            break;

        isSame = true;
        F = E;
    }

    return E;
}

cv::Mat auto_hole( cv::Mat I )
{
    cv::Mat I_c = 255 - I;
    auto F = I_c.clone();
    const int nRow = F.rows;
    const int nCol = F.cols;

    F.forEach<uchar>( [&]( Pixel& p, const int* i ) {
        p = (( i[0] == 0 || i[0] == nRow - 1 || i[1] == 0 || i[1] == nCol - 1 ))
                ? p
                : 0;
    } );

    auto R = geodesic_reconst_D( F, I_c, rectSE( 3 ) );
    cv::Mat H = 255 - R;
    return H;
}

cv::Mat border_clean( cv::Mat I )
{
    auto F = I.clone();
    const int nRow = F.rows;
    const int nCol = F.cols;

    F.forEach<uchar>( [&]( Pixel& p, const int* i ) {
        p = (( i[0] == 0 || i[0] == nRow - 1 || i[1] == 0 || i[1] == nCol - 1 ))
                ? p
                : 0;
    } );

    return I - geodesic_reconst_D(F,I,rectSE(3));
}