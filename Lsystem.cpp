#include "Lsystem.h"
#include "Rule.h" //#include "Rule.h" will be enough, when I write it!!!
#include "Token.h"
#include "Cmd.h"
#include "Lexer.h"
#include "Parser.h"

#include <algorithm>
using std::find;

#include <stdexcept>
using std::runtime_error;

#include <fstream>
using std::ifstream;

void assertdatatoken(const Token &t) {
if(t.iseof()) throw runtime_error("Unexpected end of file");
if(t.iseol()) throw runtime_error("Unexpected end of line"); 
}

Cmdcont readrule(Lexer &lex)
{
    Cmdcont retval;
    Token t=lex.nexttoken();
    while(!t.iseol()) {
        assertdatatoken(t);
        if (t.getdata()=="left") {
            t=lex.nexttoken();
            assertdatatoken(t);
            retval.push_back(make_shared<Rotatecmd>(Parser(t.getdata()).parse()));
        }
        else if (t.getdata() == "right"){
            t=lex.nexttoken();
            assertdatatoken(t);
            retval.push_back(make_shared<Rotatecmd>(Parser("-("+t.getdata()+")").parse()));
        }
        else if (t.getdata() == "|" || t.getdata() == "flip")
            retval.push_back(make_shared<Flipcmd>());
        else if (t.getdata() == "[" || t.getdata() == "push")
            retval.push_back(make_shared<Pushcmd>());
        else if (t.getdata() == "]" || t.getdata() == "pop")
            retval.push_back(make_shared<Popcmd>());
        else { //A rule
            bool rev=false;
            bool flip=false;
            string thisrule=t.getdata();
            while (thisrule[0]=='~' || thisrule[0]=='|') {
                if(thisrule[0]=='~') rev=true;
                if(thisrule[0]=='|') flip=true;
                thisrule=thisrule.substr(1); //all but first character
            }
            if(thisrule[thisrule.length()-1]=='@') {
                thisrule = thisrule.substr(0,thisrule.length()-1);
                t=lex.nexttoken();
                assertdatatoken(t);
                retval.push_back(make_shared<Rulecmd>(thisrule,rev,flip,Parser(t.getdata()).parse()));
            }
            else
                retval.push_back(make_shared<Rulecmd>(thisrule,rev,flip));
        }
        t = lex.nexttoken();
    }
    return retval;
}

void readruleoptions(Lexer &lex, Rule &r)
{
Token t=lex.nexttoken();

while(t.isdata())
   {
   if (t.getdata()=="drawmethod")
      {
      t=lex.nexttoken();
      assertdatatoken(t);
      if(t.getdata()=="drop")
         r.setdrawmethod(Rule::DROP);
      else if (t.getdata()=="normal")
         r.setdrawmethod(Rule::NORM);
      else if (t.getdata()=="rectangle")
         r.setdrawmethod(Rule::RECT);
      else if (t.getdata()=="invisible")
         r.setdrawmethod(Rule::INVIS);
      else if (t.getdata()=="midpoint")
         r.setdrawmethod(Rule::MIDPT);
      else
         throw runtime_error("Unexpected draw method " + t.getdata());
      }
   else if (t.getdata()=="dropangle")
      {
      t=lex.nexttoken();
      assertdatatoken(t);
      r.dropangle = Parser(t.getdata()).parse();//!!! Rule member function? (then can take away friendship)
      }
   else if (t.getdata()=="dropdistance")
      {
      t=lex.nexttoken();
      assertdatatoken(t);
      r.dropdistance = Parser(t.getdata()).parse();//!!! Rule member function? (then can take away friendship)
      }
   else if (t.getdata()=="rectwidth")
      {
      t=lex.nexttoken();
      assertdatatoken(t);
      r.rectwidth = Parser(t.getdata()).parse();//!!! Rule member function? (then can take away friendship)
      } //!!!fix similarity of last three elseifs 
   else if (t.getdata()=="info")
      {
      t=lex.nexttoken();
      assertdatatoken(t);
      r.info = t.getdata();
      }//!!!even this is pretty similar,
   else if (t.getdata()=="localscale")
      {
      t=lex.nexttoken();
      assertdatatoken(t);
      r.scalefac = Parser(t.getdata()).parse();//!!! Rule member function? (then can take away friendship)
      }
   else throw runtime_error("Unexpected option " + t.getdata());
   t = lex.nexttoken();
   }
}

vector<Lsystem> readlsystemfile(const string &configfilename)
{
    ifstream in(configfilename);
    if(!in.good()) {
        in.clear();
        in.open("../"+configfilename);
    }
    if(!in.good())
        throw runtime_error("Couldn't open file " + configfilename + "\n");
    Lexer lex(in);
    
    vector<Lsystem> thelsystems;
    while(1)
    {
        string systemname;
        bool activerule=true;
        Ruletable table;
        string startrule;
        Exprtype expressions;
        
        Token t;
        do t=lex.nexttoken(); while (t.iseol());//get a non EOL token
        if(t.iseof()) {
            if (thelsystems.empty()) 
                throw runtime_error("No L systems in file "+ configfilename);
            else
                return thelsystems;
        }
        systemname=t.getdata();
        t=lex.nexttoken();
        while(t.isdata())
        {
            if(t.getdata() == "inactive")   
            {
                activerule = false;
                t=lex.nexttoken();
            }
            else throw runtime_error("unrecognized option after system name: " + systemname);
        }
        if (t.iseof())
            throw runtime_error("Unexpected end of file during " + systemname);
        // t is EOL
        t=lex.nexttoken();
        while(t.isdata()) //read (rule, rule option, or expression) line
        {
            string rulename = t.getdata();
            try
            {
                if(rulename[0]=='$') 
                {
                    t=lex.nexttoken();
                    assertdatatoken(t);
                    if (t.getdata() != "=")
                        throw runtime_error("Expected '='");
                    t=lex.nexttoken();
                    assertdatatoken(t);
                    expressions[rulename]=Parser(t.getdata()).parse();
                    t=lex.nexttoken();
                    if(t.isdata())
                        throw runtime_error("Unexpected additional characters "+t.getdata());
                }
                else
                {
                    if (startrule.empty()) startrule=rulename;
                    t=lex.nexttoken();
                    assertdatatoken(t);
                    if(t.getdata()==":")
                        table[rulename].setcmds(readrule(lex));
                    else if(t.getdata()=="?")
                        readruleoptions(lex,table[rulename]);
                    else throw runtime_error("Expected option line or rule definition");
                }
            }
            catch (runtime_error &error)
            {throw runtime_error(string(error.what()) + " in " + systemname +" rule "+rulename);}
            t=lex.nexttoken();
        }
        thelsystems.push_back(Lsystem(systemname,activerule,table,startrule,expressions));
        if(t.iseof())
            return thelsystems;
    }
}
