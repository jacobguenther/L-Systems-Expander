#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <memory>
#include <string>
#include <utility>

class Parsenode;

using Consttype = std::map<std::string, double>;
using Exprtype = std::map<std::string, std::shared_ptr<Parsenode>>;

class Context {
   public:
    Context() = default;
	Context(Consttype _c, Exprtype _e);
    const Consttype constants;
    const Exprtype expressions;
};
#endif
