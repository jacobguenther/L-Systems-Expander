#include "Parsenode.h"

double Ternopnode::eval(const Context& cc) {
	return bool(pred->eval(cc)) ? trueval->eval(cc) : falseval->eval(cc);
}

double Binopnode::eval(const Context& cc) {
	double ll = left->eval(cc);
	double rr = right->eval(cc);
	switch (op) {
	case '+':
		return ll + rr;
	case '-':
		return ll - rr;
	case '*':
		return ll * rr;
	case '/':
		return ll / rr;
	case '%':
		return static_cast<int>(ll) % static_cast<int>(rr);
	case '&':
		return static_cast<double>(bool(ll) && bool(rr));
	case '|':
		return static_cast<double>(bool(ll) || bool(rr));
		//         case '=': return ll==rr; // !!! Shouldn't compare
		//         doubles with ==
	case 'g':
		return static_cast<double>(ll >= rr);
	case 'l':
		return static_cast<double>(ll <= rr);
		//         case 'n': return ll!=rr; // !!! Shouldn't compare
		//         doubles with ==
	case '<':
		return static_cast<double>(ll < rr);
	case '>':
		return static_cast<double>(ll > rr);
	default:
		throw std::runtime_error(
				std::string { "unrecognized binary operator " } + op);
	}
}

Binopnode::Binopnode(char _o, ParsenodePtr _l, ParsenodePtr _r) :
		op(_o), left(std::move(_l)), right(std::move(_r)) {
}

Ternopnode::Ternopnode(ParsenodePtr _p, ParsenodePtr _t, ParsenodePtr _f) :
		pred(std::move(_p)), trueval(std::move(_t)), falseval(std::move(_f)) {
}

Unopnode::Unopnode(char _o, ParsenodePtr _c) :
		op(_o), child(std::move(_c)) {
}

double Unopnode::eval(const Context& cc) {
	if (op == '!')
		return static_cast<double>(!static_cast<bool>(child->eval(cc)));

	if (op == '-')
		return -child->eval(cc);

	if (op == '+')
		return child->eval(cc);

	throw std::runtime_error(std::string("unrecognized unary operator ") + op);
}

Numnode::Numnode(double _v) :
		val(_v) {
}

double Numnode::eval(const Context&) {
	return val;
}

double Idnode::eval(const Context& cc) {
	//add RND, srand, rand?!!!
	static const double DEG = 180 / M_PI;
	if (name == "acos")
		return acos(pp[0]->eval(cc));

	if (name == "acosd")
		return DEG * acos(pp[0]->eval(cc));

	if (name == "asin")
		return asin(pp[0]->eval(cc));

	if (name == "asind")
		return DEG * asin(pp[0]->eval(cc));

	if (name == "atan")
		return atan(pp[0]->eval(cc));

	if (name == "atand")
		return DEG * atan(pp[0]->eval(cc));

	if (name == "atan2")
		return atan2(pp[0]->eval(cc), pp[1]->eval(cc));

	if (name == "atan2d")
		return DEG * atan2(pp[0]->eval(cc), pp[1]->eval(cc));

	if (name == "ceil")
		return ceil(pp[0]->eval(cc));

	if (name == "cos")
		return cos(pp[0]->eval(cc));

	if (name == "cosd")
		return cos(pp[0]->eval(cc) / DEG);

	if (name == "cosh")
		return cosh(pp[0]->eval(cc));

	if (name == "exp")
		return exp(pp[0]->eval(cc));

	if (name == "fabs")
		return fabs(pp[0]->eval(cc));

	if (name == "floor")
		return floor(pp[0]->eval(cc));

	if (name == "fmod")
		return fmod(pp[0]->eval(cc), pp[1]->eval(cc));

	if (name == "log")
		return log(pp[0]->eval(cc));

	if (name == "log10")
		return log10(pp[0]->eval(cc));

	if (name == "pow")
		return pow(pp[0]->eval(cc), pp[1]->eval(cc));

	if (name == "sin")
		return sin(pp[0]->eval(cc));

	if (name == "sind")
		return sin(pp[0]->eval(cc) / DEG);

	if (name == "sinh")
		return sinh(pp[0]->eval(cc));

	if (name == "sqrt")
		return sqrt(pp[0]->eval(cc));

	if (name == "tan")
		return tan(pp[0]->eval(cc));

	if (name == "tand")
		return tan(pp[0]->eval(cc) / DEG);

	auto ii = cc.constants.find(name);
	if (ii != cc.constants.end())
		return ii->second;

	auto jj = cc.expressions.find(name);
	if (jj != cc.expressions.end())
		return jj->second->eval(cc);

	throw std::runtime_error(name + ": undefined identifier.");
}

Idnode::Idnode(std::string _n, std::vector<ParsenodePtr> _p) :
		name(std::move(_n)), pp(std::move(_p)) {
}
