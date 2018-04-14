#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <memory>
#include <string>

class Parsenode;

typedef std::map<std::string, double> Consttype;
typedef std::map<std::string, std::shared_ptr<Parsenode> > Exprtype;

class Context {
   public:
    Context(const Consttype &_c, const Exprtype &_e) : constants(_c), expressions(_e) {}
    const Consttype constants;
    const Exprtype expressions;
};

#endif
