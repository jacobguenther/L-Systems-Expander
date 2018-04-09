#ifndef PARSENODE_H
#define PARSENODE_H

#include <cmath>

#include <stdexcept>
using std::runtime_error;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Context.h"
#include <iostream>
//!!! Maybe turn off warnings in this file wrt using double as bool and comparing double ==

class Parsenode
{
public:
   Parsenode(){}
   virtual double eval(const Context &)=0;
   virtual ~Parsenode(){}
private:
   static bool lastconstruct;
};

class Ternopnode : public Parsenode
{
public:
   Ternopnode(Parsenode *_p,Parsenode *_t,Parsenode *_f)
      :pred(_p),trueval(_t),falseval(_f){}
   double eval(const Context &cc)
      {
      return bool(pred->eval(cc)) ? trueval->eval(cc) : falseval->eval(cc);
      }
   ~Ternopnode()
      {
      delete pred; delete trueval; delete falseval;
      }
private:
   Parsenode *pred;
   Parsenode *trueval;
   Parsenode *falseval;
};

class Binopnode : public Parsenode
{
public:
   Binopnode(char _o, Parsenode *_l, Parsenode *_r)
      :op(_o),left(_l),right(_r){}
   double eval(const Context &cc)
      {
      double ll=left->eval(cc);
      double rr=right->eval(cc);
      switch(op)
         {
         case '+': return ll+rr;
         case '-': return ll-rr;
         case '*': return ll*rr;
         case '/': return ll/rr;
         case '%': return static_cast<int>(ll) % static_cast<int>(rr);
         case '&': return bool(ll)&&bool(rr);
         case '|': return bool(ll)||bool(rr);
//         case '=': return ll==rr; // !!! Shouldn't compare doubles with ==
         case 'g': return ll>=rr;
         case 'l': return ll<=rr;
//         case 'n': return ll!=rr; // !!! Shouldn't compare doubles with ==
         case '<': return ll<rr;
         case '>': return ll>rr;
         default : throw runtime_error(string("unrecognized binary operator ")+op);
         }
      }
   ~Binopnode()
      {
      delete left;
      delete right;
      }
private:
   char op;
   Parsenode *left;
   Parsenode *right;
};

class Unopnode : public Parsenode
{
public:
   Unopnode(char _o, Parsenode *_c)
      :op(_o),child(_c){}
   double eval(const Context &cc)
      {
      if(op=='!') return !static_cast<int>(child->eval(cc));
      if(op=='-') return -child->eval(cc);
      if(op=='+') return child->eval(cc);
      throw runtime_error(string("unrecognized unary operator ")+op);
      }
   ~Unopnode()
      {
      delete child;
      }
private:
   char op;
   Parsenode *child;
};

class Numnode : public Parsenode
{
public: 
   Numnode(double _v):val(_v){}
   double eval(const Context &){return val;}
private:
   double val;
};

class Idnode : public Parsenode
{
public:
   Idnode(const string &_n,const vector<Parsenode *> &_p)
      :name(_n),pp(_p){}
   double eval(const Context &cc)
      {//add RND, srand, rand?!!!
      static const double DEG=180/M_PI;
      if(name=="acos")  return acos(pp[0]->eval(cc));
      if(name=="acosd") return DEG*acos(pp[0]->eval(cc));
      if(name=="asin")  return asin(pp[0]->eval(cc));
      if(name=="asind") return DEG*asin(pp[0]->eval(cc));
      if(name=="atan")  return atan(pp[0]->eval(cc));
      if(name=="atand") return DEG*atan(pp[0]->eval(cc));
      if(name=="atan2") return atan2(pp[0]->eval(cc),pp[1]->eval(cc));
      if(name=="atan2d")return DEG*atan2(pp[0]->eval(cc),pp[1]->eval(cc));
      if(name=="ceil")  return ceil(pp[0]->eval(cc));
      if(name=="cos")   return cos(pp[0]->eval(cc));
      if(name=="cosd")  return cos(pp[0]->eval(cc)/DEG);
      if(name=="cosh")  return cosh(pp[0]->eval(cc));
      if(name=="exp")   return exp(pp[0]->eval(cc));
      if(name=="fabs")  return fabs(pp[0]->eval(cc));
      if(name=="floor") return floor(pp[0]->eval(cc));
      if(name=="fmod")  return fmod(pp[0]->eval(cc),pp[1]->eval(cc));
      if(name=="log")   return log(pp[0]->eval(cc));
      if(name=="log10") return log10(pp[0]->eval(cc));
      if(name=="pow")   return pow(pp[0]->eval(cc),pp[1]->eval(cc));
      if(name=="sin")   return sin(pp[0]->eval(cc));
      if(name=="sind")  return sin(pp[0]->eval(cc)/DEG);
      if(name=="sinh")  return sinh(pp[0]->eval(cc));
      if(name=="sqrt")  return sqrt(pp[0]->eval(cc));
      if(name=="tan")   return tan(pp[0]->eval(cc));
      if(name=="tand")  return tan(pp[0]->eval(cc)/DEG);
      Consttype::const_iterator ii=cc.constants.find(name);
      if(ii!=cc.constants.end()) 
         return ii->second;
      Exprtype::const_iterator jj=cc.expressions.find(name);
      if(jj!=cc.expressions.end())
         return jj->second->eval(cc);
      else throw runtime_error(name+": undefined identifier."); 
      // return 0;
      }
   ~Idnode()
      {
      for(vector<Parsenode *>::const_iterator ii=pp.begin(); ii != pp.end(); ++ii)
         delete *ii;
      }
private:
   string name;
   vector<Parsenode *> pp;
};
#endif
