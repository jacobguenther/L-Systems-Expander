//
//  RuleState.hpp
//  lsystems
//
//  Created by Chris Hartman on 4/19/18.
//

#ifndef RuleState_hpp
#define RuleState_hpp
#include "Cmd.h"

class Rulerunner;
class Rule;

class Rulestate {
    friend class Rulerunner;
    
public:
    Rulestate(const Rule& myRule, bool isReversed, double oldScale,
              double flipFactor);
    
    bool hasNoMoreCommands();
    
    void runCurrentCommandOn(Rulerunner &target);
    
    bool _isReversed;
    double _flipFactor;
    const Rule &_myRule;
    Commands::const_iterator _nextCommand;
    double _oldScale;
};

#endif /* RuleState_hpp */
