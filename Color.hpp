//
//  Color.hpp
//  lsystems
//
//  Created by Chris Hartman on 4/24/18.
//

#ifndef Color_hpp
#define Color_hpp
#include <array>

class Color {
public:
    static Color colorCircle(double t);
    void glSet();
private:
    std::array<float,3> _rgb{{0.0,0.0,0.0}};
};
#endif /* Color_hpp */
