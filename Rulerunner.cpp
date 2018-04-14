#include "Rulerunner.h"
#include "Cmd.h"

#include <stdexcept>
using std::logic_error;

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

void Rulerunner::handlerule(const string &rr, bool rulerev, bool ruleflip,
                            double localscale) {
    bool willflip = rulerev ^ ruleflip;
    if (rulestates.size() >= maxdepth || localscale * turtles.top().getscale() < minscale) {
        Motion temp;
        temp.frompt = turtles.top().getposition();
        turtles.top().forward(localscale);  //!!!  Here is where we fix issue #5
        temp.topt = turtles.top().getposition();
        switch (therules[rr].drawmethod) {  //!!!Use a factory here!
            case Rule::NORM:
                agraphic.reset(new Linegraphic(temp));
                break;
            case Rule::MIDPT:
                agraphic.reset(new Dropgraphic(temp, 0, .5));
                break;
            case Rule::INVIS:
                agraphic.reset(new Invisgraphic);
                break;
            case Rule::RECT:
                agraphic.reset(new Linegraphic(temp));
                break;  //!!!
            case Rule::DROP:
                double ffac = (willflip ? -1 : 1) * turtles.top().getflip();
                agraphic.reset(new Dropgraphic(temp,
                                               ffac * therules[rr].cacheddropangle,
                                               therules[rr].cacheddropdistance));
                break;  //!!!
        }
    } else {
        bool currentlybw = rulestates.empty() ? false : rulestates.top().backwards;
        rulestates.push(Rulestate(&therules[rr], currentlybw ^ rulerev,
                                  turtles.top().getscale(), willflip));
        double newscalefac = localscale * therules[rr].cachedscalefac;
        turtles.top().scaleby(newscalefac);
        if (willflip) turtles.top().flip();
    }
}

void Rulestate::doit(Rulerunner *towho) {
    if (backwards)
        (*--mypos)->execute(towho);
    else
        (*mypos++)->execute(towho);
}

std::shared_ptr<Graphic> Rulerunner::nextpoint() {
    if (agraphic.get() == 0)
        throw logic_error("Called nextpoint() on a Rulerunner with no graphic ready\n");
    std::shared_ptr<Graphic> temp(agraphic);
    makeapoint();
    return temp;
}

void Rulerunner::makeapoint() {
    while (!rulestates.empty() && !agraphic.get()) {  //Go until we hit a turtle forward, or we're done
        if (rulestates.top().done()) {
            turtles.top().setscale(rulestates.top().oldscale);
            if (rulestates.top().flipped) turtles.top().flip();
            rulestates.pop();
        } else
            rulestates.top().doit(this);
    }
    if (rulestates.empty() && !agraphic.get())
        finished = true;
}

void Rulerunner::drawnextpoint() {
#ifdef _DEBUG
    if (agraphic.get() == 0)
        throw logic_error("Called drawnextpoint() on a Rulerunner with no graphic ready\n");
#endif
    agraphic->draw();
    agraphic.reset();
    makeapoint();
}
