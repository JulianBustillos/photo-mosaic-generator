#include "ColorUtils.h"
#include <numbers>


double ColorUtils::sRGBToLinear(double c)
{
    return (c <= 0.04045) ? c / 12.92 : std::pow((c + 0.055) / 1.055, 2.4);
}

void ColorUtils::RGBToOKLab(uchar rgb_r, uchar rgb_g, uchar rgb_b, double& OKLab_L, double& OKLab_a, double& OKLab_b) 
{
    // sRGB -> linear RGB
    const double r = sRGBToLinear((double)rgb_r / 255.0);
    const double g = sRGBToLinear((double)rgb_g / 255.0);
    const double b = sRGBToLinear((double)rgb_b / 255.0);

    // Linear RGB -> LMS
    const double l = 0.4122214708 * r + 0.5363325363 * g + 0.0514459929 * b;

    const double m = 0.2119034982 * r + 0.6806995451 * g + 0.1073969566 * b;

    const double s = 0.0883024619 * r + 0.2817188376 * g + 0.6299787005 * b;

    // Non-linearity
    const double l_ = std::cbrt(l);
    const double m_ = std::cbrt(m);
    const double s_ = std::cbrt(s);

    // LMS -> OKLab
    OKLab_L = 0.2104542553 * l_ + 0.7936177850 * m_ - 0.0040720468 * s_;
    OKLab_a = 1.9779984951 * l_ - 2.4285922050 * m_ + 0.4505937099 * s_;
    OKLab_b = 0.0259040371 * l_ + 0.7827717662 * m_ - 0.8086757660 * s_;
}
