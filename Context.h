#ifndef CONTEXT_H
#define CONTEXT_H

#include <memory>
#include <string>
#include <unordered_map>

class Parsenode;

using Consttype = std::unordered_map<std::string, double>;
using Exprtype = std::unordered_map<std::string, std::shared_ptr<Parsenode>>;

class Context {
   public:
    Context() = default;
	Context(Consttype _c, Exprtype _e);
    const Consttype constants;
    const Exprtype expressions;
};
#endif
