#ifndef RULERUNNER_H
#define RULERUNNER_H

#include "Context.h"
#include "Lsystem.h"
#include "Parser.h"
#include "Rule.h"
#include "Turtle.h"
#include <list>
#include <map>
#include <stack>
#include <stdexcept>
#include <string>


class Lexer;
class Lsystem;

class Rulerunner {
friend class Command;
friend class PopCommand; //Turtle pops need to know about drawstrategies
friend class RuleCommand; //Rules need to call handlerule
friend class LinesDrawStrategy;
friend class DropDrawStrategy;public:
    Rulerunner(Lsystem &l, int maxdepth, double minscale, const Consttype &c);
    void draw();
private:
    struct RuleInvocation {
        const Rule &rule;
        bool rulerev;
        bool ruleflip;
        double atScale;
        int depth;
    };
    
    bool isDeepEnough(int depth);
    void handlerule(const RuleInvocation &ri);
    void doCommand(Command &c, const RuleInvocation &ri);

    Ruletable &_therules; //Would like to be const, but calculating/caching parameters changes
    //!!! can access throu _lSystem?
    std::string _startrule;
    const Context _context;
    int _maxdepth;
    Turtle _turtle;
    bool _backwards=false;  //!!!Still used?
    double _minscale;
    Lsystem &_lSystem; //Think about this. Do we really want parent pointers?!!!
};
#endif
