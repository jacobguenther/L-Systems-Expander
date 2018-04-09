#ifndef CMD_H
#define CMD_H

#include <string>
using std::string;

#include <list>
using std::list;

#include "xboost_shared_ptr.hpp"
using xboost::shared_ptr;

//class Parsenode;
#include "Parser.h"
#include "Context.h"
#include "Rulerunner.h"

class Cmd
{
public:
   Cmd(){}
   virtual void execute(Rulerunner *master)=0;
   virtual ~Cmd(){}
   virtual void cachevalue(const Context &){}
private:
   static bool lastconstruct;
};

typedef list<shared_ptr<Cmd> > Cmdcont;

class Rotatecmd : public Cmd
{
public:
   Rotatecmd(shared_ptr<Parsenode> _a):angle(_a){}
   virtual void execute(Rulerunner *master)
      {master->turtles.top().rotate(cachedangle);}
   virtual void cachevalue(const Context &cc)
      {cachedangle = angle->eval(cc);}
private:
   shared_ptr<Parsenode> angle;
   double cachedangle;
};

class Flipcmd : public Cmd
{
public:
   virtual void execute(Rulerunner *master)
      {master->turtles.top().flip();}
};

class Pushcmd : public Cmd
{
public:
   virtual void execute(Rulerunner *master)
      {master->pushturtle();}
};

class Popcmd : public Cmd
{
public:
   virtual void execute(Rulerunner *master)
      { master->popturtle();
        Dropgraphic::haveapt=false;
      }
};

class Rulecmd : public Cmd
{
public:
   Rulecmd(const string &_m, bool _r=false, bool _f=false,
           shared_ptr<Parsenode> _s = Parser("1").parse())
      :myrule(_m),rev(_r),flip(_f),scale(_s){}
   virtual void execute(Rulerunner *master)
      { master->handlerule(myrule,rev,flip,cachedscale);}
   virtual void cachevalue(const Context &cc)
      { cachedscale = scale->eval(cc); }
private:
   const string myrule;
   bool rev;
   bool flip;
   shared_ptr<Parsenode> scale;
   double cachedscale;
};
#endif
