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
   public:
    Lsystem(std::string_view name, Lexer &lex);
	const std::string& getname() const;
	bool isactive() const;
    const Exprtype & getExpressions() const;
    const Ruletable & getRules() const;
    const std::string & startRule() const;
    const std::string & getDrawStrategy() const;
   private:
    void readSystemOptions(Lexer &lex);
    
    std::string _name;
    std::string _info;
    bool active=true;
    Ruletable _rules;
    std::string startrule;
    Exprtype expressions;
    std::string _drawStrategy;
    std::vector<ParsenodePtr> _drawStrategyParameters;
};

std::vector<Lsystem> readlsystemfile(const std::string &configfilename =
		"config file.txt");

#endif
