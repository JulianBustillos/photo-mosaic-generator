#pragma once

#include "Descriptor.h"
#include <opencv2/opencv.hpp>
#include <bitset>


namespace ImageUtils
{
    constexpr int BlurNbBoxes = 3;
    constexpr int HashSize = 8;
    constexpr int HashBits = 2 * HashSize * 8;

    using Hash = std::bitset<HashBits>;

    enum Filter
    {
        AREA,
        BICUBIC,
        LANCZOS
    };

    void resample(cv::Mat& target, const cv::Size targetSize, const cv::Mat& source, Filter filter);
    void resample(cv::Mat& target, const cv::Size targetSize, const cv::Mat& source, const cv::Rect& box, Filter filter);

    void gaussianBlur(uchar* image, const cv::Size& size, double sigma);

    void DHash(const cv::Mat& image, Hash& hash);

    void guidedFiltering(std::vector<double>& filtered, const std::vector<double>& image, const std::vector<double>& guide, const cv::Size& size, int radius, double epsilon);
};

