#include "Context.h"

Context::Context(Consttype _c, Exprtype _e) :
		constants(std::move(_c)), expressions(std::move(_e)) {
}
