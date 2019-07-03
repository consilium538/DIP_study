#include "basic.hpp"

uchar threshold(double unlim)
{
    if (unlim <= 0) return 0;
    else if (unlim >= 255) return 255;
    else return (uchar)std::round(unlim);
}
