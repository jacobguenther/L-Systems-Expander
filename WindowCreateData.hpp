// WindowCreateData.hpp

#ifndef WINDOW_CREATE_DATA_HPP
#define WINDOW_CREATE_DATA_HPP

#include <string>
#include <utility>

struct WindowCreateData {
    explicit WindowCreateData(
        std::string windowTitle = "L' Systems",
        std::pair<double, double> windowDimensions = {800, 800},
        uint numMultisamplingSamples = 0,
        bool shouldSyncFrame = true);
    std::string _windowTitle;
    std::pair<int, int> _windowDimensions;
    uint _numMultisamplingSamples;
    bool _shouldSyncFrame;
};

#endif

