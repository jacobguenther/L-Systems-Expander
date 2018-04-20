//
//  RuleState.cpp
//  lsystems
//
//  Created by Chris Hartman on 4/19/18.
//

#include "RuleState.h"
#include "Rule.h"
#include "Rulerunner.h"

Rulestate::Rulestate(const Rule& myRule, bool isReversed, double scaleFactor,
                     double flipFactor) :
_isReversed(isReversed), _flipFactor(flipFactor), _myRule(myRule),_nextCommand(_isReversed ? _myRule.cmds.cend() : _myRule.cmds.cbegin()), _scaleFactor(
scaleFactor) {
}

bool Rulestate::hasNoMoreCommands() {
    return _nextCommand
    == (_isReversed ? _myRule.cmds.begin() : _myRule.cmds.end());
}

void Rulestate::runCurrentCommandOn(Rulerunner &target) {
    if (_isReversed)
        (*--_nextCommand)->executeOn(target);
    else
        (*_nextCommand++)->executeOn(target);
}

void Rulerunner::draw() {
    while (!_rulestates.empty()) {
        if (_rulestates.top().hasNoMoreCommands())
            pop();
        else
            _rulestates.top().runCurrentCommandOn(*this);
    }
}
