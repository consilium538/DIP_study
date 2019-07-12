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
#define SKIP_GLOBAL
//#define SKIP_OTSU
#define SKIP_EDGE_GRAD
#define SKIP_LAPLACE_GRAD
#define SKIP_MULTIPLE_GRAD
#define SKIP_VARIABLE
#endif  // RUN_ALL

void catCPUID( std::ostream& out )
{
    char CPUBrandString[0x40];
    unsigned int CPUInfo[4] = {0, 0, 0, 0};

    __cpuid( 0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3] );
    unsigned int nExIds = CPUInfo[0];

    memset( CPUBrandString, 0, sizeof( CPUBrandString ) );

    for ( unsigned int i = 0x80000000; i <= nExIds; ++i )
    {
        __cpuid( i, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3] );

        if ( i == 0x80000002 )
            memcpy( CPUBrandString, CPUInfo, sizeof( CPUInfo ) );
        else if ( i == 0x80000003 )
            memcpy( CPUBrandString + 16, CPUInfo, sizeof( CPUInfo ) );
        else if ( i == 0x80000004 )
            memcpy( CPUBrandString + 32, CPUInfo, sizeof( CPUInfo ) );
    }

    out << "CPU Type: " << CPUBrandString << std::endl;
}

#endif  // MAIN_HPP
