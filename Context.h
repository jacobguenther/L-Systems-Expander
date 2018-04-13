#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <memory>
#include <string>
using std::shared_ptr;

using std::map;
using std::string;

class Parsenode;

typedef map<string, double> Consttype;
typedef map<string, shared_ptr<Parsenode> > Exprtype;

class Context {
   public:
    Context(const Consttype &_c, const Exprtype &_e) : constants(_c), expressions(_e) {}
    const Consttype constants;
    const Exprtype expressions;
};

#endif
