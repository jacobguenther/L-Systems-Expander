#ifndef PARSENODE_H
#define PARSENODE_H

#include "Context.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

//!!! Maybe turn off warnings in this file wrt using double as bool and comparing double ==

using ParsenodePtr = std::unique_ptr<Parsenode>;

class Parsenode {
   public:
    Parsenode() = default;
    virtual ~Parsenode() = default;
    Parsenode(const Parsenode&) = delete;
    Parsenode& operator=(const Parsenode&) = delete;
    Parsenode(Parsenode&&) = delete;
    Parsenode& operator=(Parsenode&&) = delete;

    virtual double eval(const Context & cc) = 0;

   private:
    static bool lastconstruct;
};

class Ternopnode : public Parsenode {
   public:
	Ternopnode(ParsenodePtr _p, ParsenodePtr _t, ParsenodePtr _f);
	double eval(const Context& cc) override;

   private:
    ParsenodePtr pred;
    ParsenodePtr trueval;
    ParsenodePtr falseval;
};

class Binopnode : public Parsenode {
   public:
	Binopnode(char _o, ParsenodePtr _l, ParsenodePtr _r);
	double eval(const Context& cc) override;

   private:
    char op;
    ParsenodePtr left;
    ParsenodePtr right;
};

class Unopnode : public Parsenode {
   public:
	Unopnode(char _o, ParsenodePtr _c);
	double eval(const Context& cc) override;

   private:
    char op;
    ParsenodePtr child;
};

class Numnode : public Parsenode {
   public:
	explicit Numnode(double _v);
	double eval(const Context& cc) override;

   private:
    double val;
};

class Idnode : public Parsenode {
   public:
	Idnode(std::string _n, std::vector<ParsenodePtr> _p);
	double eval(const Context& cc) override;

   private:
    std::string name;
    std::vector<ParsenodePtr> pp;
};
#endif
