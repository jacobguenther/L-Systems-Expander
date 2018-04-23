#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "Context.h"
#include "DrawStrategy.hpp"
#include "Rule.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

class Lexer;

class Lsystem {
    friend class Rulerunner;
    friend class PopCommand;
    friend class RuleCommand;
   public:
    Lsystem(std::string_view name, Lexer &lex);
	const std::string& getname();
	bool isactive() const;

   private:
    void readSystemOptions(Lexer &lex);
    
    std::string _name;
    std::string _info;
    DrawStrategyPtr _drawStrategy;
    bool active=true;
    Ruletable table;
    std::string startrule;
    Exprtype expressions;
};

std::vector<Lsystem> readlsystemfile(const std::string &configfilename =
		"config file.txt");

#endif
