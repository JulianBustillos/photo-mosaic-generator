#include "Descriptor.h"


void Descriptor::compute(const cv::Mat& image)
{
    level1.compute(image);
    level2.compute(image);
    level4.compute(image);
}

double Descriptor::distance(const Descriptor& other) const {
    return level4.meanDistance(other.level4);
}