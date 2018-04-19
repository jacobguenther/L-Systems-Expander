// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Turtle.h"

using std::make_unique;
#include <iostream>
using std::cout;
using std::endl;

std::unique_ptr<Graphic> Turtle::draw(const Rule &rule, double flipFactor, double distance) {
    Motion temp;
    temp.frompt = getposition();
    forward(distance);
    temp.topt = getposition();
    switch (rule.drawmethod) {  //!!!Use a factory here!
        case Rule::NORM:
            return make_unique<Linegraphic>(temp);
        case Rule::MIDPT:
            return make_unique<Dropgraphic>(temp, 0, .5);
        case Rule::INVIS:
            return make_unique<Invisgraphic>();
        case Rule::RECT:
            return make_unique<Linegraphic>(temp);
        case Rule::DROP:
            return make_unique<Dropgraphic>(temp,
                                            flipFactor* getflip() * rule.cacheddropangle,
                                            rule.cacheddropdistance);
        case Rule::WRITE:
            cout << "[" << getflip() << "," << getscale() << "] " << rule.info << " distance: " << distance << " flipFactor: " << flipFactor << endl;
            return make_unique<Linegraphic>(temp);
    }
}
