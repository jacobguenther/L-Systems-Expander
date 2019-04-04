// WindowCreateData.hpp

#ifndef WINDOW_CREATE_DATA_HPP
#define WINDOW_CREATE_DATA_HPP

#include <string>
#include <utility>

struct WindowCreateData {
    explicit WindowCreateData(
        std::string windowTitle_ = "L' Systems",
        std::pair<double, double> windowDimensions_ = {800, 800},
        uint multisamplingSamples_ = 0,
        bool syncFrame_ = true)
            : windowTitle(windowTitle_)
            , windowDimensions{windowDimensions_}
            , multisamplingSamples{multisamplingSamples_}
            , syncFrame{syncFrame_}
    {}
    std::string windowTitle;
    std::pair<int, int> windowDimensions;
    uint multisamplingSamples;
    bool syncFrame;
};

#endif

