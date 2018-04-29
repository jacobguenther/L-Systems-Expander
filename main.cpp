// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Cmd.h"
#include "Context.h"
#include "GlutFunctions.hpp"
#include "Lsystem.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    try {
        cout << "Use <, > (no shift necessary) to change fractal level." << endl;
        cout << "Use i,j,k,l to translate and -,= to scale" << endl;
        cout << "Use [ and ] to change threshold for dynamic level drawing." << endl;
        cout << "Use u and v to change parameter for parameterized fractals." << endl;

        readtheconfigfile();
        runGlut(argc,argv);
        
        return 0;
    } catch (std::exception &error) {
        std::cerr << error.what() << endl;
        return 1;
    }
}
