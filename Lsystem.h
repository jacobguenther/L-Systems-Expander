#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <string>
using std::string;

#include <map>
using std::map;

#include <vector>
using std::vector;

#include "Rule.h"

typedef map<string,Rule> Ruletable;

#include "Context.h"

class Lsystem
{
friend class Rulerunner;
public:
   Lsystem(string _n, bool _a, const Ruletable & _t, string _s, Exprtype _e) :
      name(_n), active(_a), table(_t), startrule(_s), expressions(_e){}
   const string & getname(){return name;}
   bool isactive() const {return active;}
private:
   string name;
   bool active;
   Ruletable table;
   string startrule;
   Exprtype expressions;
};

vector<Lsystem> readlsystemfile(const string &configfilename = "config file.txt");

#endif

