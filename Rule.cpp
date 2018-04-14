#include "Rule.h"
#include "Cmd.h"

void Rule::setdrawmethod(Method m) {
    switch (m) {
        case NORM:
            break;
        case DROP:
            if (!dropangle.get())
                dropangle = Parser("0").parse();
            if (!dropdistance.get())
                dropdistance = Parser("1").parse();
            break;
        case RECT:
            if (!rectwidth.get())
                rectwidth = Parser("0.1").parse();
            break;
        case INVIS:
        case MIDPT:
            break;
    }
    drawmethod = m;
}

void Rule::cachevalues(const Context& cc) {
    for (auto ii = cmds.begin(); ii != cmds.end(); ++ii)
        (*ii)->cachevalue(cc);
    switch (drawmethod) {
        case DROP:
            cacheddropangle = dropangle->eval(cc) * M_PI / 180.0;  //!!! cache sin and cos instead?
            cacheddropdistance = dropdistance->eval(cc);
            break;
        case RECT:
            cachedrectwidth = rectwidth->eval(cc);
            break;
        case NORM:
        case INVIS:
        case MIDPT:
            break;
    }
    cachedscalefac = scalefac.get() ? scalefac->eval(cc) : 1;
}
