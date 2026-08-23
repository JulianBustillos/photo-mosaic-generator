#pragma once

#include <opencv2/opencv.hpp>


namespace ColorUtils
{
    template< typename T>
    inline T clip(T val, T min, T max) { return (val < min) ? min : (val > max) ? max : val; };

    double sRGBToLinear(double c);
    void RGBToOKLab(uchar rgb_r, uchar rgb_g, uchar rgb_b, double& OKLab_L, double& OKLab_a, double& OKLab_b);
};

