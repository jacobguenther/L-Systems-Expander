#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "Context.h"
#include "Rule.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

class Lexer;

class Lsystem {
    friend class Rulerunner;
    enum Method {
        NONE,
        NORM,
        DROP,
        RECT,
        INVIS,
        MIDPT,
        WRITE
    } ;

   public:
    Lsystem(std::string_view name, Lexer &lex);
	const std::string& getname();
	bool isactive() const;

   private:
    void readSystemOptions(Lexer &lex);
    
    std::string _name;
    std::string _info;
    Method _drawMethod = NORM;
    ParsenodePtr _dropAngleExpression;
    ParsenodePtr _dropDistanceExpression;
    bool active=true;
    Ruletable table;
    std::string startrule;
    Exprtype expressions;
};

std::vector<Lsystem> readlsystemfile(const std::string &configfilename =
		"config file.txt");

#endif
