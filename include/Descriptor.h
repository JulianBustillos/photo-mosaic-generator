#pragma once

#include <array>

struct Descriptor
{
    static constexpr int FeatureDiv = 4;
    static constexpr int NbFeatures = 3 * FeatureDiv * FeatureDiv;

    std::array<double, NbFeatures> _features = {0};
};
