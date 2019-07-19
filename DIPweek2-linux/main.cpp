#include "main.hpp"

int main( int argv, char** argc )
{
    namespace fs = std::filesystem;

    const string inputPath = "./srcImg/";
    auto savepath = fs::path( "./dstImg/" );
    if ( !fs::exists( savepath ) )
        fs::create_directory( savepath );

    auto logpath = fs::path( "./log" );
    if ( !fs::exists( logpath ) )
        fs::create_directory( logpath );
    auto logfile = logpath / "log.txt";

    std::streambuf* log_buf;
    std::ofstream log_fs;
    if ( isLogFile )
    {
        log_fs.open( logfile.string(), std::ios::app );
        log_buf = log_fs.rdbuf();
    }
    else
    {
        log_buf = std::cout.rdbuf();
    }
    std::ostream log_out( log_buf );
    // testout << "test log" << std::endl;

    std::vector<std::tuple<cv::Mat, string>> ImgArr;

    catCPUID( log_out );
    auto now = std::time( nullptr );
    log_out << "Process start at " << std::ctime( &now ) << std::endl;

////////////////////////////////////////
#ifndef SKIP_EROSION

    log_out << "/////////////////////////////\n";

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

    log_out << "// process end! //\n" << std::endl;

#endif  // SKIP_EROSION
////////////////////////////////////////
#ifndef SKIP_DILATION  // text-broken.tif

    log_out << "/////////////////////////////\n";

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

    log_out << "// process end! //\n" << std::endl;

#endif  // SKIP_DILATION
////////////////////////////////////////
#ifndef SKIP_OPENING

    log_out << "/////////////////////////////\n";

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

    log_out << "// process end! //\n" << std::endl;

#endif  // SKIP_OPENING
////////////////////////////////////////
#ifndef SKIP_BOUNDARY

    log_out << "/////////////////////////////\n";

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
                                          lincoln_orig.channels() );
    lincoln_bound.forEach( [&]( Pixel& p, const int* i ) {
        p = cv::saturate_cast<uchar>( lincoln_orig.at<uchar>( i[0], i[1] ) -
                                      lincoln_eros.at<uchar>( i[0], i[1] ) );
    } );
    ImgArr.push_back( std::make_tuple( lincoln_bound, "lincoln_bound" ) );

    log_out << "// process end! //\n" << std::endl;

#endif  // SKIP_BOUNDARY
////////////////////////////////////////
#ifndef SKIP_HOLE  // balls-with-reflections.tif
                   // crossSE

    log_out << "/////////////////////////////\n";

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

    log_out << "// process end! //\n" << std::endl;

#endif  // SKIP_HOLE
////////////////////////////////////////
#ifndef SKIP_CONNECTED  // chickenXray.tif

    log_out << "/////////////////////////////\n";

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

    log_out << "// process end! //\n" << std::endl;

#endif  // SKIP_CONNECTED
////////////////////////////////////////
#ifndef SKIP_RECONSTRUCTION  // text.tif
                             // 25*1

    log_out << "// start of reconstruction process //\n";

    auto text_orig = cv::imread( inputPath + "text.tif", cv::IMREAD_GRAYSCALE );
    if ( text_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }

    // cv::resize( text_orig, text_orig, Size(), 0.5, 0.5, INTER_NEAREST );
    ImgArr.push_back( std::make_tuple( text_orig, "text_orig" ) );

#ifndef SKIP_RECON_OPEN

    log_out << "// start of reconstruction_open process //\n";
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

    log_out << "// reconstruction_open process end! //\n";
    log_out << "time taken by reconstruction_open process is : " << fixed
            << time_taken_recon_open << setprecision( 9 );
    log_out << " sec\n" << endl;

#endif  // SKIP_RECON_OPEN

#ifndef SKIP_RECON_FILL

    log_out << "// start of reconstruction_fill process //\n";
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

    log_out << "// reconstruction_fill process end! //\n";
    log_out << "time taken by reconstruction_fill process is : " << fixed
            << time_taken_recon_fill << setprecision( 9 );
    log_out << " sec\n" << endl;

#endif  // SKIP_RECON_FILL

#ifndef SKIP_RECON_BORDER

    log_out << "// start of reconstruction_border process //\n";
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

    log_out << "// reconstruction_border process end! //\n";
    log_out << "time taken by reconstruction_border process is : " << fixed
            << time_taken_recon_border << setprecision( 9 );
    log_out << " sec\n" << endl;

#endif  // SKIP_RECON_BORD

    log_out << "// reconstruction process end! //\n" << std::endl;

    // img_cat( ImgArr );

#endif  // SKIP_RECONSTRUCTION
////////////////////////////////////////
#ifndef SKIP_GLOBAL  // fingerprint.tif

    log_out << "// start of global thresholding process //\n";

    auto finger_gray_orig =
        cv::imread( inputPath + "fingerprint.tif", cv::IMREAD_GRAYSCALE );
    if ( finger_gray_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back( std::make_tuple( finger_gray_orig, "finger_gray_orig" ) );
    for ( auto it : histogram(finger_gray_orig) )
    {
        log_out << it << " ";
    }
    auto finger_global_th = global_threshold( finger_gray_orig );
    log_out << "\n" << finger_global_th << std::endl;

    auto finger_global_seg =
        intensityTransform( finger_gray_orig, thresholding( finger_global_th ) );
    ImgArr.push_back(
        std::make_tuple( finger_global_seg, "finger_global_seg" ) );

    log_out << "// process end! //\n" << std::endl;

    // img_cat( ImgArr );

#endif  // SKIP_GLOBAL
////////////////////////////////////////
#ifndef SKIP_OTSU  // polymercell.tif

    log_out << "// start of otsu process //\n";

    auto polymercell_orig =
        cv::imread( inputPath + "polymercell.tif", cv::IMREAD_GRAYSCALE );
    if ( polymercell_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back( std::make_tuple( polymercell_orig, "polymercell_orig" ) );

    auto start_otsu = chrono::high_resolution_clock::now();

    for ( auto it : histogram(polymercell_orig) )
    {
        log_out << it << " ";
    }
    auto polymercell_otsu_th = otsu_threshold( polymercell_orig );
    log_out << "\n" << polymercell_otsu_th << std::endl;
    auto polymercell_otsu_seg = intensityTransform(
        polymercell_orig, thresholding( polymercell_otsu_th ) );
    ImgArr.push_back(
        std::make_tuple( polymercell_otsu_seg, "polymercell_otsu_seg" ) );

    auto end_otsu = chrono::high_resolution_clock::now();
    double time_taken_otsu =
        std::chrono::duration_cast<chrono::nanoseconds>( end_otsu - start_otsu )
            .count();
    time_taken_otsu *= 1e-9;

    log_out << "// otsu process end! //\n";
    log_out << "time taken by otsu process is : " << fixed << time_taken_otsu
            << setprecision( 9 );
    log_out << " sec\n" << endl;

    // img_cat( ImgArr );

#endif  // SKIP_OTSU
////////////////////////////////////////
#ifndef SKIP_SMOOTH_GLOBAL

    log_out << "// start of global thresholding by smoothing process //\n";

    auto septagon_orig =
        cv::imread( inputPath + "septagon.tif", cv::IMREAD_GRAYSCALE );
    if ( septagon_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    // ImgArr.push_back( std::make_tuple( septagon_orig, "septagon_orig" ) );

    auto start_smooth_global = chrono::high_resolution_clock::now();

    auto septagon_gauss = testGaussian( septagon_orig, 0.0, 50.0 );
    ImgArr.push_back( std::make_tuple( septagon_gauss, "septagon_gauss_50" ) );

    cv::Mat septagon_double;
    septagon_gauss.convertTo( septagon_double, CV_64F );
    cv::Mat septagon_smooth;
    conv2d( septagon_double, box_filter( 5 ), Padding::replicate )
        .convertTo( septagon_smooth, CV_8U );
    ImgArr.push_back( std::make_tuple( septagon_smooth, "septagon_smooth_5" ) );

    auto septagon_global_th = global_threshold( septagon_smooth );
    auto septagon_global_seg = intensityTransform(
        septagon_smooth, thresholding( septagon_global_th ) );
    ImgArr.push_back(
        std::make_tuple( septagon_global_seg, "septagon_global_seg" ) );

    auto end_smooth_global = chrono::high_resolution_clock::now();
    double time_taken_smooth_global =
        std::chrono::duration_cast<chrono::nanoseconds>( end_smooth_global -
                                                         start_smooth_global )
            .count();
    time_taken_smooth_global *= 1e-9;

    log_out << "// global thresholding by smoothing process end! //\n";
    log_out << "time taken by global thresholding by smoothing process is : "
            << fixed << time_taken_smooth_global << setprecision( 9 );
    log_out << " sec\n" << endl;

    // img_cat( ImgArr );

#endif  // SKIP_SMOOTH_GLOBAL
////////////////////////////////////////
#ifndef SKIP_EDGE_GRAD  // septagon-small.tif

    log_out << "// start of EDGE_GRAD process //\n";

    auto septagon_small_orig =
        cv::imread( inputPath + "septagon-small.tif", cv::IMREAD_GRAYSCALE );
    if ( septagon_small_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back(
        std::make_tuple( septagon_small_orig, "septagon_small_orig" ) );

    auto start_edge_grad = chrono::high_resolution_clock::now();

    auto sept_small_grad = grad2d( septagon_small_orig );
    cv::Mat sept_small_grad_norm;
    sept_small_grad.convertTo( sept_small_grad_norm, CV_8U, 0.25 );
    ImgArr.push_back(
        std::make_tuple( sept_small_grad_norm, "sept_small_grad_norm" ) );
    auto sept_small_grad_hist = histogram_ex( sept_small_grad );

    // for( auto it: sept_small_grad_hist )
    //     log_out << it << " ";
    // log_out << std::endl;

    const int sept_small_pixels =
        septagon_small_orig.rows * septagon_small_orig.cols;
    int sept_small_grad_th = 1023;
    int sept_small_grad_cum = 0;
    for ( ; sept_small_grad_th > 0; sept_small_grad_th-- )
    {
        sept_small_grad_cum += sept_small_grad_hist[sept_small_grad_th];
        if ( sept_small_grad_cum > (double)sept_small_pixels * 0.003 )
            break;
    }

    cv::Mat sept_small_index = sept_small_grad > sept_small_grad_th;
    ImgArr.push_back( std::make_tuple( sept_small_index, "sept_small_index" ) );

    auto sept_small_hist =
        indexed_histogram( septagon_small_orig, sept_small_index );
    auto sept_small_th =
        otsu_threshold_indexed( septagon_small_orig, sept_small_index );
    auto sept_small_seg = intensityTransform( septagon_small_orig,
                                              thresholding( sept_small_th ) );
    ImgArr.push_back(
        std::make_tuple( sept_small_seg, "sept_small_edge_grad" ) );

    auto end_edge_grad = chrono::high_resolution_clock::now();
    double time_taken_edge_grad =
        std::chrono::duration_cast<chrono::nanoseconds>( end_edge_grad -
                                                         start_edge_grad )
            .count();
    time_taken_edge_grad *= 1e-9;

    log_out << "// EDGE_GRAD process end! //\n";
    log_out << "time taken by EDGE_GRAD process is : " << fixed
            << time_taken_edge_grad << setprecision( 9 );
    log_out << " sec\n" << endl;

    // img_cat( ImgArr );

#endif  // SKIP_EDGE_GRAD
////////////////////////////////////////
#ifndef SKIP_EDGE_LAPLACE  // yeast-cells.tif

    log_out << "// start of EDGE_LAPLACE process //\n";

    auto yeast_cell_orig =
        cv::imread( inputPath + "yeast-cells.tif", cv::IMREAD_GRAYSCALE );
    if ( yeast_cell_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back( std::make_tuple( yeast_cell_orig, "yeast_cell_orig" ) );

    auto start_edge_laplace = chrono::high_resolution_clock::now();

    cv::Mat yeast_cell_double;
    yeast_cell_orig.convertTo( yeast_cell_double, CV_64F );
    cv::Mat yeast_cell_laplace = cv::abs(
        conv2d( yeast_cell_double, laplacian_filter( LaplaceOption::eight ),
                Padding::replicate ) );
    cv::Mat yeast_cell_laplace_norm;
    yeast_cell_laplace.convertTo( yeast_cell_laplace_norm, CV_8U, 0.25 );
    ImgArr.push_back(
        std::make_tuple( yeast_cell_laplace_norm, "yeast_cell_laplace_norm" ) );
    auto yeast_cell_laplace_hist = histogram_ex( yeast_cell_laplace );

    // for( auto it: yeast_cell_laplace_hist )
    //     log_out << it << " ";
    // log_out << std::endl;

    const int yeast_cell_pixels = yeast_cell_orig.rows * yeast_cell_orig.cols;
    int yeast_cell_laplace_th = 1023;
    int yeast_cell_laplace_cum = 0;
    for ( ; yeast_cell_laplace_th > 0; yeast_cell_laplace_th-- )
    {
        yeast_cell_laplace_cum +=
            yeast_cell_laplace_hist[yeast_cell_laplace_th];
        if ( yeast_cell_laplace_cum > (double)yeast_cell_pixels * 0.003 )
            break;
    }

    cv::Mat yeast_cell_index = yeast_cell_laplace > yeast_cell_laplace_th;
    ImgArr.push_back( std::make_tuple( yeast_cell_index, "yeast_cell_index" ) );

    auto yeast_cell_hist =
        indexed_histogram( yeast_cell_orig, yeast_cell_index );
    auto yeast_cell_th =
        otsu_threshold_indexed( yeast_cell_orig, yeast_cell_index );
    auto yeast_cell_seg =
        intensityTransform( yeast_cell_orig, thresholding( yeast_cell_th ) );
    ImgArr.push_back(
        std::make_tuple( yeast_cell_seg, "yeast_cell_edge_laplace" ) );

    auto end_edge_laplace = chrono::high_resolution_clock::now();
    double time_taken_edge_laplace =
        std::chrono::duration_cast<chrono::nanoseconds>( end_edge_laplace -
                                                         start_edge_laplace )
            .count();
    time_taken_edge_laplace *= 1e-9;

    log_out << "// EDGE_LAPLACE process end! //\n";
    log_out << "time taken by EDGE_LAPLACE process is : " << fixed
            << time_taken_edge_laplace << setprecision( 9 );
    log_out << " sec\n" << endl;

    // img_cat( ImgArr );

#endif  // SKIP_EDGE_LAPLACE
////////////////////////////////////////
#ifndef SKIP_MULTIPLE_TH  // iceberg.tif

    log_out << "// start of MULTIPLE_TH process //\n";

    auto iceberg_orig =
        cv::imread( inputPath + "iceberg.tif", cv::IMREAD_GRAYSCALE );
    if ( iceberg_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back( std::make_tuple( iceberg_orig, "iceberg_orig" ) );

    auto start_multiple_th = chrono::high_resolution_clock::now();

    auto iceberg_otsu_th = multi_otsu_threshold( iceberg_orig );
    auto iceberg_otsu_seg =
        intensityTransform( iceberg_orig, thresholding_2( iceberg_otsu_th ) );
    ImgArr.push_back( std::make_tuple( iceberg_otsu_seg, "iceberg_otsu_seg" ) );

    auto end_multiple_th = chrono::high_resolution_clock::now();
    double time_taken_multiple_th =
        std::chrono::duration_cast<chrono::nanoseconds>( end_multiple_th -
                                                         start_multiple_th )
            .count();
    time_taken_multiple_th *= 1e-9;

    log_out << "// MULTIPLE_TH process end! //\n";
    log_out << "time taken by MULTIPLE_TH process is : " << fixed
            << time_taken_multiple_th << setprecision( 9 );
    log_out << " sec\n" << endl;

    // img_cat( ImgArr );

#endif  // SKIP_MULTIPLE_TH
////////////////////////////////////////
#ifndef SKIP_VARIABLE_IMG_LOCAL  // yeast-cells.tif

    log_out << "// start of VARIABLE_IMG_LOCAL process //\n";

    const double alpha = 60.0;
    const double beta = 1.5;

    auto yeast_orig =
        cv::imread( inputPath + "yeast-cells.tif", cv::IMREAD_GRAYSCALE );
    if ( yeast_orig.empty() )
    {
        cout << "image load failed!" << endl;
        return -1;
    }
    ImgArr.push_back( std::make_tuple( yeast_orig, "yeast_orig" ) );

    auto start_img_local = chrono::high_resolution_clock::now();

    auto yeast_local_var = local_var( yeast_orig );
    cv::Mat yeast_local_var_norm;
    yeast_local_var.convertTo(yeast_local_var_norm, CV_8U, 20.0);
    ImgArr.push_back( std::make_tuple( yeast_local_var_norm, "yeast_local_var" ) );

    auto yeast_hist = histogram( yeast_orig );
    unsigned long long int yeast_sum = 0;
    for ( int i = 0; i < 256; i++ )
        yeast_sum += i * yeast_hist[i];
    double yeast_mean =
        (double)yeast_sum / (double)( yeast_orig.rows * yeast_orig.cols );

    cv::Mat yeast_local_th = cv::Mat_<uchar>( yeast_orig.size() );
    yeast_local_th.forEach<uchar>( [&]( uchar& p, const int* i ) {
        p = (( (double)yeast_orig.at<uchar>( i[0], i[1] ) >
            alpha * yeast_local_var.at<double>( i[0], i[1] ) ) &&
            ( (double)yeast_orig.at<uchar>( i[0], i[1] ) >
              beta * yeast_mean )) ? 255 : 0;
    } );
    ImgArr.push_back( std::make_tuple( yeast_local_th, "yeast_local_th" ) );

    auto end_img_local = chrono::high_resolution_clock::now();
    double time_taken_img_local =
        std::chrono::duration_cast<chrono::nanoseconds>( end_img_local -
                                                         start_img_local )
            .count();
    time_taken_img_local *= 1e-9;

    log_out << "// VARIABLE_IMG_LOCAL process end! //\n";
    log_out << "time taken by VARIABLE_IMG_LOCAL process is : " << fixed
            << time_taken_img_local << setprecision( 9 );
    log_out << " sec\n" << endl;

#endif  // SKIP_VARIABLE_IMG_LOCAL
////////////////////////////////////////
#ifndef SKIP_VARIABLE_MOVING_AVG  // text-sineshade.tif, text-spotshade.tif

    log_out << "// start of VARIABLE_MOVING_AVG process //\n";

    log_out << "// process end! //\n" << std::endl;

#endif  // SKIP_VARIABLE_MOVING_AVG
    log_out << "/////////////////////////////\n" << std::endl;

    img_save(ImgArr, savepath.string(), ".png");

    return 0;
}

//    cv::imwrite( savepath + "moon-blurred.tif", TransformedImg );
