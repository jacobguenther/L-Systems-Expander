// WindowCreateData.cpp

#include "WindowCreateData.hpp"

WindowCreateData::WindowCreateData(
    std::string windowTitle,
    std::pair<double, double> windowDimensions,
    uint numMultisamplingSamples,
    bool shouldSyncFrame)
        : _windowTitle(windowTitle)
        , _windowDimensions{windowDimensions}
        , _numMultisamplingSamples{numMultisamplingSamples}
        , _shouldSyncFrame{shouldSyncFrame}
{}
