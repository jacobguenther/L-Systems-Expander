#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "Context.h"
#include "Rule.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

class Lsystem {
    friend class Rulerunner;

   public:
    Lsystem(std::string _n, bool _a, Ruletable _t, std::string _s, Exprtype _e) : name(std::move(_n)), active(_a), table(std::move(_t)), startrule(std::move(_s)), expressions(std::move(_e)) {}
    const std::string &getname() { return name; }
    bool isactive() const { return active; }

   private:
    std::string name;
    bool active;
    Ruletable table;
    std::string startrule;
    Exprtype expressions;
};

std::vector<Lsystem> readlsystemfile(const std::string &configfilename = "config file.txt");

#endif
