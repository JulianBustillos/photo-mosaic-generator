#include "Descriptor.h"


void Descriptor::compute(const cv::Mat& image)
{
    level1.compute(image);
    level2.compute(image);
    level4.compute(image);
}

double Descriptor::distance(const Descriptor& other) const {
    return 0.2  * level1.meanDistance(other.level1) + 
           0.3  * level2.meanDistance(other.level2) +
           0.5  * level4.meanDistance(other.level4) + 
           0.1  * level1.stdDistance(other.level1) +
           0.1  * level2.stdDistance(other.level2) + 
           0.05 * level4.stdDistance(other.level4);
}