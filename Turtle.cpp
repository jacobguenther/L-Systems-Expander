#include "Turtle.h"

std::shared_ptr<Graphic> Turtle::draw(const Rule &rule) {
    Motion temp;
    temp.frompt = getposition();
    forward(1);  //!!!  Here is where we fix issue #5
    temp.topt = getposition();
    switch (rule.drawmethod) {  //!!!Use a factory here!
        case Rule::NORM:
            return std::make_shared<Linegraphic>(temp);
        case Rule::MIDPT:
            return std::make_shared<Dropgraphic>(temp, 0, .5);
        case Rule::INVIS:
            return std::make_shared<Invisgraphic>();
        case Rule::RECT:
            return std::make_shared<Linegraphic>(temp);
        case Rule::DROP:
            return std::make_shared<Dropgraphic>(temp,
                                                      getflip() * rule.cacheddropangle,
                                                      rule.cacheddropdistance);
    }
}
