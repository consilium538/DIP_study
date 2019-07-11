#ifndef MAIN_HPP
#define MAIN_HPP

// #define RUN_ALL

#ifndef RUN_ALL
#define SKIP_EROSION
#define SKIP_DILATION
#define SKIP_OPENING
#define SKIP_BOUNDARY
#define SKIP_HOLE
#define SKIP_CONNECTED
#define SKIP_RECONSTRUCTION
/////
#define SKIP_RECON_OPEN
#define SKIP_RECON_FILL
#define SKIP_RECON_BORDER
/////
// #define SKIP_GLOBAL
#define SKIP_OTSU
#define SKIP_EDGE_GRAD
#define SKIP_LAPLACE_GRAD
#define SKIP_MULTIPLE_GRAD
#define SKIP_VARIABLE
#endif  // RUN_ALL

#endif  // MAIN_HPP
