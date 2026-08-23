#pragma once

#include <array>
#include <opencv2/opencv.hpp>
#include "ColorUtils.h"


struct ColorStats
{
    double meanL = 0.0;
    double meanA = 0.0;
    double meanB = 0.0;

    double stdL = 0.0;
    double stdA = 0.0;
    double stdB = 0.0;
};

template <int Div> 
class DescriptorLevel 
{
public:
    void compute(const cv::Mat& image);
    double meanDistance(const DescriptorLevel& other) const;
    double stdDistance(const DescriptorLevel& other) const;

private:
    static constexpr int Div = Div;
    static constexpr int CellCount = Div * Div;
    std::array<ColorStats, CellCount> cells{};
};

class Descriptor
{
public:
    void compute(const cv::Mat& image);
    double distance(const Descriptor& other) const;

private:
    DescriptorLevel<1> level1;
    DescriptorLevel<2> level2;
    DescriptorLevel<4> level4;
};


template <int Div>
void DescriptorLevel<Div>::compute(const cv::Mat& image) {
    for (auto& cell : cells)
        cell = {};

    std::array<double, CellCount> sumSqL{};
    std::array<double, CellCount> sumSqA{};
    std::array<double, CellCount> sumSqB{};

    const int width = image.cols;
    const int height = image.rows;

    const int blockWidth = (width + Div - 1) / Div;
    const int blockHeight = (height + Div - 1) / Div;

    for (int i = 0; i < height; i++) {
        const cv::Vec3b* src = image.ptr<cv::Vec3b>(i);

        for (int j = 0; j < width; j++) {
            double L, a, b;

            ColorUtils::RGBToOKLab(src[j][2], src[j][1], src[j][0], L, a, b);

            const int blockRow = i / blockHeight;
            const int blockCol = j / blockWidth;
            const int block = blockRow * Div + blockCol;

            auto& cell = cells[block];
            cell.meanL += L;
            cell.meanA += a;
            cell.meanB += b;

            sumSqL[block] += L * L;
            sumSqA[block] += a * a;
            sumSqB[block] += b * b;
        }
    }

    for (int blockRow = 0; blockRow < Div; blockRow++) {
        for (int blockCol = 0; blockCol < Div; blockCol++) {
            const int block = blockRow * Div + blockCol;

            const int x0 = blockCol * blockWidth;
            const int x1 = std::min(x0 + blockWidth, width);

            const int y0 = blockRow * blockHeight;
            const int y1 = std::min(y0 + blockHeight, height);

            const int pixelCount = (x1 - x0) * (y1 - y0);

            auto& cell = cells[block];
            cell.meanL /= pixelCount;
            cell.meanA /= pixelCount;
            cell.meanB /= pixelCount;

            const double varL = sumSqL[block] / pixelCount - cell.meanL * cell.meanL;
            const double varA = sumSqA[block] / pixelCount - cell.meanA * cell.meanA;
            const double varB = sumSqB[block] / pixelCount - cell.meanB * cell.meanB;

            cell.stdL = std::sqrt(std::max(0.0, varL));
            cell.stdA = std::sqrt(std::max(0.0, varA));
            cell.stdB = std::sqrt(std::max(0.0, varB));
        }
    }
}

template <int Div> 
double DescriptorLevel<Div>::meanDistance(const DescriptorLevel& other) const {
    double sumDist = 0.;
    for (int i = 0; i < CellCount; i++) {
        const auto& cell = cells[i];
        const auto& otherCell = other.cells[i];
        double dL = cell.meanL - otherCell.meanL;
        double dA = cell.meanA - otherCell.meanA;
        double dB = cell.meanB - otherCell.meanB;
        double sqDist = dL * dL + dA * dA + dB * dB;
        sumDist += sqrt(sqDist);
    }

    return sumDist / CellCount;
}

template <int Div> 
double DescriptorLevel<Div>::stdDistance(const DescriptorLevel& other) const {
    double sumDist = 0.;
    for (int i = 0; i < CellCount; i++) {
        const auto& cell = cells[i];
        const auto& otherCell = other.cells[i];
        double dL = cell.stdL - otherCell.stdL;
        double dA = cell.stdA - otherCell.stdA;
        double dB = cell.stdB - otherCell.stdB;
        double sqDist = dL * dL + dA * dA + dB * dB;
        sumDist += sqrt(sqDist);
    }

    return sumDist / CellCount;
}