#include "main.hpp"

int main( int argv, char** argc )
{
    const string inputPath = "./srcImg/";
    const string savepath = "./tmpImg/";
    std::vector<std::tuple<cv::Mat, string>> ImgArr;

    catCPUID( std::cout );

////////////////////////////////////////
#ifndef SKIP_EROSION

    std::cout << "/////////////////////////////\n";

    auto circuitmask_orig =
        cv::imread( inputPath + "circuitmask.tif", cv::IMREAD_GRAYSCALE );
    cv::resize( circuitmask_orig, circuitmask_orig, Size(), 0.5, 0.5,
                INTER_NEAREST );
    ImgArr.push_back( std::make_tuple( circuitmask_orig, "circuitmask_orig" ) );

    if ( circuitmask_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto circuitmask_11 = erosion( circuitmask_orig, rectSE( 11 ) );
    ImgArr.push_back( std::make_tuple( circuitmask_11, "circuitmask_11" ) );

    auto circuitmask_15 = erosion( circuitmask_orig, rectSE( 15 ) );
    ImgArr.push_back( std::make_tuple( circuitmask_15, "circuitmask_15" ) );

    auto circuitmask_45 = erosion( circuitmask_orig, rectSE( 45 ) );
    ImgArr.push_back( std::make_tuple( circuitmask_45, "circuitmask_45" ) );

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_EROSION
////////////////////////////////////////
#ifndef SKIP_DILATION  // text-broken.tif

    std::cout << "/////////////////////////////\n";

    auto text_broken_orig =
        cv::imread( inputPath + "text-broken.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( text_broken_orig, "text_broken_orig" ) );

    if ( text_broken_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto text_broken_dil = dilation( text_broken_orig, rectSE( 3 ) );
    ImgArr.push_back( std::make_tuple( text_broken_dil, "text_broken_dil" ) );

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_DILATION
////////////////////////////////////////
#ifndef SKIP_OPENING

    std::cout << "/////////////////////////////\n";

    auto finger_orig =
        cv::imread( inputPath + "fingerprint-noisy.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( finger_orig, "finger_orig" ) );

    if ( finger_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto se = rectSE( 3 );

    auto finger_eroded = erosion( finger_orig, se );
    ImgArr.push_back( std::make_tuple( finger_eroded, "finger_eroded" ) );

    auto finger_open = opening( finger_orig, se );
    ImgArr.push_back( std::make_tuple( finger_open, "finger_open" ) );

    auto finger_open_close = closing( finger_open, se );
    ImgArr.push_back(
        std::make_tuple( finger_open_close, "finger_open_close" ) );

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_OPENING
////////////////////////////////////////
#ifndef SKIP_BOUNDARY

    std::cout << "/////////////////////////////\n";

    auto lincoln_orig =
        cv::imread( inputPath + "lincoln.tif", cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( lincoln_orig, "lincoln_orig" ) );

    if ( lincoln_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    auto lincoln_eros = erosion( lincoln_orig, rectSE( 3 ) );
    auto lincoln_bound = cv::Mat_<uchar>( lincoln_orig.size(),
                                          ball_lincoln_origorig.channels() );
    lincoln_bound.forEach( [&]( Pixel& p, const int* i ) {
        p = cv::saturate_cast<uchar>( lincoln_orig.at<uchar>( i[0], i[1] ) -
                                      lincoln_eros.at<uchar>( i[0], i[1] ) );
    } );
    ImgArr.push_back( std::make_tuple( lincoln_bound, "lincoln_bound" ) );

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_BOUNDARY
////////////////////////////////////////
#ifndef SKIP_HOLE  // balls-with-reflections.tif
                   // crossSE

    std::cout << "/////////////////////////////\n";

    auto ball_orig = cv::imread( inputPath + "balls-with-reflections.tif",
                                 cv::IMREAD_GRAYSCALE );
    ImgArr.push_back( std::make_tuple( ball_orig, "ball_orig" ) );

    if ( ball_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    std::vector<std::tuple<int, int>> hole_init = {
        {60, 50},   {180, 45},  {363, 39},  {93, 159},  {178, 203},
        {262, 148}, {414, 233}, {461, 142}, {104, 299}, {236, 308},
        {386, 371}, {489, 357}, {56, 451},  {235, 460}, {388, 371},
        {489, 358}, {58, 451},  {234, 459}, {505, 490}, {399, 471}};

    auto ball_hole = cv::Mat( ball_orig.size(), CV_8UC1, cv::Scalar( 0 ) );

    for ( auto it : hole_init )
        ball_hole.at<uchar>( std::get<1>( it ), std::get<0>( it ) ) = 255;

    cv::Mat ball_dilated;
    auto ball_filled = cv::Mat( ball_orig.size(), CV_8UC1, cv::Scalar( 0 ) );
    bool noChange = true;

    for ( int i = 0; i < 50; i++ )
    {
        ball_dilated = dilation( ball_hole, crossSE() );

        ball_filled.forEach<uchar>( [&]( Pixel& p, const int* i ) {
            p = ball_orig.at<uchar>( i[0], i[1] ) == 0 &&
                        ball_dilated.at<uchar>( i[0], i[1] ) == 255
                    ? 255
                    : 0;
            if ( p != ball_hole.at<uchar>( i[0], i[1] ) )
                noChange = false;
        } );

        if ( noChange == true )
            break;

        noChange = true;
        ball_hole.release();
        ball_hole = ball_filled.clone();
    }

    auto ball_completed = cv::Mat( ball_orig.size(), CV_8UC1, cv::Scalar( 0 ) );
    ball_completed.forEach<uchar>( [&]( Pixel& p, const int* i ) {
        p = cv::saturate_cast<uchar>( ball_orig.at<uchar>( i[0], i[1] ) +
                                      ball_hole.at<uchar>( i[0], i[1] ) );
    } );
    ImgArr.push_back( std::make_tuple( ball_completed, "ball_completed" ) );

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_HOLE
////////////////////////////////////////
#ifndef SKIP_CONNECTED  // chickenXray.tif

    std::cout << "/////////////////////////////\n";

    auto chicken_orig = cv::imread( inputPath + "chickenXray-thresholded.tif",
                                    cv::IMREAD_GRAYSCALE );
    cv::resize( chicken_orig, chicken_orig, Size(), 0.5, 0.5, INTER_NEAREST );
    ImgArr.push_back( std::make_tuple( chicken_orig, "chicken_orig" ) );

    if ( chicken_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    std::vector<std::tuple<int, int>> cc_init = {{0, 0}};

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_CONNECTED
////////////////////////////////////////
#ifndef SKIP_RECONSTRUCTION  // text.tif
                             // 25*1

    std::cout << "// start of reconstruction process //\n";

    auto text_orig = cv::imread( inputPath + "text.tif", cv::IMREAD_GRAYSCALE );
    if ( text_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    // cv::resize( text_orig, text_orig, Size(), 0.5, 0.5, INTER_NEAREST );
    ImgArr.push_back( std::make_tuple( text_orig, "text_orig" ) );

#ifndef SKIP_RECON_OPEN

    std::cout << "// start of reconstruction_open process //\n";
    auto start_recon_open = chrono::high_resolution_clock::now();

    auto text_eros = erosion( text_orig, rectSE( 51, 1 ) );
    auto text_opening = opening( text_orig, rectSE( 51, 1 ) );
    auto text_reconst =
        geodesic_reconst_D( text_opening, text_orig, rectSE( 3 ) );
    ImgArr.push_back( std::make_tuple( text_reconst, "text_reconst" ) );

    auto end_recon_open = chrono::high_resolution_clock::now();
    double time_taken_recon_open =
        std::chrono::duration_cast<chrono::nanoseconds>( end_recon_open -
                                                         start_recon_open )
            .count();
    time_taken_recon_open *= 1e-9;

    std::cout << "// reconstruction_open process end! //\n";
    std::cout << "time taken by reconstruction_open process is : " << fixed
              << time_taken_recon_open << setprecision( 9 );
    std::cout << " sec\n" << endl;

#endif  // SKIP_RECON_OPEN

#ifndef SKIP_RECON_FILL

    std::cout << "// start of reconstruction_fill process //\n";
    auto start_recon_fill = chrono::high_resolution_clock::now();

    cv::Mat text_invert = 255 - text_orig;
    auto text_filled = auto_hole( text_orig );
    ImgArr.push_back( std::make_tuple( text_filled, "text_filled" ) );

    auto end_recon_fill = chrono::high_resolution_clock::now();
    double time_taken_recon_fill =
        std::chrono::duration_cast<chrono::nanoseconds>( end_recon_fill -
                                                         start_recon_fill )
            .count();
    time_taken_recon_fill *= 1e-9;

    std::cout << "// reconstruction_fill process end! //\n";
    std::cout << "time taken by reconstruction_fill process is : " << fixed
              << time_taken_recon_fill << setprecision( 9 );
    std::cout << " sec\n" << endl;

#endif  // SKIP_RECON_FILL

#ifndef SKIP_RECON_BORDER

    std::cout << "// start of reconstruction_border process //\n";
    auto start_recon_border = chrono::high_resolution_clock::now();

    auto text_border_clean = border_clean( text_orig );
    cv::Mat text_border = text_orig - text_border_clean;
    ImgArr.push_back(
        std::make_tuple( text_border_clean, "text_border_clean" ) );

    auto end_recon_border = chrono::high_resolution_clock::now();
    double time_taken_recon_border =
        std::chrono::duration_cast<chrono::nanoseconds>( end_recon_border -
                                                         start_recon_border )
            .count();
    time_taken_recon_border *= 1e-9;

    std::cout << "// reconstruction_border process end! //\n";
    std::cout << "time taken by reconstruction_border process is : " << fixed
              << time_taken_recon_border << setprecision( 9 );
    std::cout << " sec\n" << endl;

#endif  // SKIP_RECON_BORD

    std::cout << "// reconstruction process end! //\n" << std::endl;

    img_cat( ImgArr );

#endif  // SKIP_RECONSTRUCTION
////////////////////////////////////////
#ifndef SKIP_GLOBAL  // fingerprint.tif

    std::cout << "/////////////////////////////\n";

    auto finger_orig =
        cv::imread( inputPath + "fingerprint.tif", cv::IMREAD_GRAYSCALE );
    if ( finger_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back( std::make_tuple( finger_orig, "finger_orig" ) );
    auto finger_global_th = global_threshold( finger_orig );

    auto finger_global_seg =
        intensityTransform( finger_orig, thresholding( finger_global_th ) );
    ImgArr.push_back(
        std::make_tuple( finger_global_seg, "finger_global_seg" ) );

    std::cout << "// process end! //\n" << std::endl;

    img_cat( ImgArr );

#endif  // SKIP_GLOBAL
////////////////////////////////////////
#ifndef SKIP_OTSU  // polymercell.tif

    std::cout << "// start of otsu process //\n";

    auto polymercell_orig =
        cv::imread( inputPath + "polymercell.tif", cv::IMREAD_GRAYSCALE );
    if ( polymercell_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back( std::make_tuple( polymercell_orig, "finger_orig" ) );

    auto start_otsu = chrono::high_resolution_clock::now();

    auto polymercell_otsu_th = otsu_threshold( polymercell_orig );
    auto polymercell_otsu_seg = intensityTransform(
        polymercell_orig, thresholding( polymercell_otsu_th ) );
    ImgArr.push_back(
        std::make_tuple( polymercell_otsu_seg, "polymercell_otsu_seg" ) );

    auto end_otsu = chrono::high_resolution_clock::now();
    double time_taken_otsu =
        std::chrono::duration_cast<chrono::nanoseconds>( end_otsu - start_otsu )
            .count();
    time_taken_otsu *= 1e-9;

    std::cout << "// process end! //\n";
    std::cout << "time taken by otsu process is : " << fixed << time_taken_otsu
              << setprecision( 9 );
    std::cout << " sec\n" << endl;

    img_cat( ImgArr );

#endif  // SKIP_OTSU
////////////////////////////////////////
#ifndef SKIP_EDGE_GRAD  //

    std::cout << "/////////////////////////////\n";

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_EDGE_GRAD
////////////////////////////////////////
#ifndef SKIP_LAPLACE_GRAD  //

    std::cout << "/////////////////////////////\n";

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_LAPLACE_GRAD
////////////////////////////////////////
#ifndef SKIP_MULTIPLE_GRAD  //

    std::cout << "/////////////////////////////\n";

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_MULTIPLE_GRAD
////////////////////////////////////////
#ifndef SKIP_VARIABLE  //

    std::cout << "/////////////////////////////\n";

    std::cout << "// process end! //\n" << std::endl;

#endif  // SKIP_VARIABLE
        ////////////////////////////////////////

    return 0;
}

//    cv::imwrite( savepath + "moon-blurred.tif", TransformedImg );
