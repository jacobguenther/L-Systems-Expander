#include "Lsystem.h"
#include "Cmd.h"
#include "Lexer.h"
#include "Parser.h"
#include "Rule.h"  //#include "Rule.h" will be enough, when I write it!!!
#include "Token.h"

using std::vector;
using std::string;

#include <stdexcept>
using std::runtime_error;

#include <fstream>
using std::ifstream;

#include <memory>
using std::make_shared;

#include <string>
using std::string_view;

void assertdatatoken(const Token &t) {
    if (t.iseof()) throw runtime_error("Unexpected end of file");
    if (t.iseol()) throw runtime_error("Unexpected end of line");
}

Commands readrule(Lexer &lex) {
    Commands retval;
    Token t = lex.nexttoken();
    while (!t.iseol()) {
        assertdatatoken(t);
        if (t.getdata() == "left") {
            t = lex.nexttoken();
            assertdatatoken(t);
            retval.push_back(make_shared<RotateCommand>(Parser(t.getdata()).parse()));
        } else if (t.getdata() == "right") {
            t = lex.nexttoken();
            assertdatatoken(t);
            retval.push_back(make_shared<RotateCommand>(Parser("-(" + t.getdata() + ")").parse()));
        } else if (t.getdata() == "|" || t.getdata() == "flip")
            retval.push_back(make_shared<FlipCommand>());
        else if (t.getdata() == "[" || t.getdata() == "push")
            retval.push_back(make_shared<PushCommand>());
        else if (t.getdata() == "]" || t.getdata() == "pop")
            retval.push_back(make_shared<PopCommand>());
        else {  //A rule
            bool rev = false;
            bool flip = false;
            string_view thisrule = t.getdata();
            while (thisrule[0] == '~' || thisrule[0] == '|') {
                if (thisrule[0] == '~') rev = true;
                if (thisrule[0] == '|') flip = true;
                thisrule = thisrule.substr(1);  //all but first character
            }
            if (thisrule[thisrule.length() - 1] == '@') {
                thisrule = thisrule.substr(0, thisrule.length() - 1);
                t = lex.nexttoken();
                assertdatatoken(t);
                retval.push_back(make_shared<RuleCommand>(string(thisrule), rev, flip, Parser(t.getdata()).parse()));
            } else
                retval.push_back(make_shared<RuleCommand>(string(thisrule), rev, flip));
        }
        t = lex.nexttoken();
    }
    return retval;
}

vector<Lsystem> readlsystemfile(const std::string &configfilename) {
    ifstream in(configfilename);
    if (!in) {
        in.clear();
        in.open("../" + configfilename);
    }
    if (!in)
        throw runtime_error("Couldn't open file " + configfilename);
    Lexer lex(in);

    vector<Lsystem> thelsystems;
    while (true) {
        string systemname;
        bool activerule = true;
        Ruletable table;
        string startrule;
        Exprtype expressions;

        Token t;
        do
            t = lex.nexttoken();
        while (t.iseol());  //get a non EOL token
        if (t.iseof()) {
            if (thelsystems.empty())
                throw runtime_error("No L systems in file " + configfilename);
            
            return thelsystems;
        }
        systemname = t.getdata();
        t = lex.nexttoken();
        while (t.isdata()) {
            if (t.getdata() == "inactive") {
                activerule = false;
                t = lex.nexttoken();
            } else
                throw runtime_error("Unrecognized option after system name: " + systemname);
        }
        if (t.iseof())
            throw runtime_error("Unexpected end of file during " + systemname);
        // t is EOL
        t = lex.nexttoken();
        while (t.isdata())  //read (rule, rule option, or expression) line
        {
            std::string rulename = t.getdata();
            try {
                if (rulename[0] == '$') {
                    t = lex.nexttoken();
                    assertdatatoken(t);
                    if (t.getdata() != "=")
                        throw runtime_error("Expected '='");
                    t = lex.nexttoken();
                    assertdatatoken(t);
                    expressions[rulename] = Parser(t.getdata()).parse();
                    t = lex.nexttoken();
                    if (t.isdata())
                        throw runtime_error("Unexpected additional characters " + t.getdata());
                } else {
                    if (startrule.empty()) startrule = rulename;
                    t = lex.nexttoken();
                    assertdatatoken(t);
                    if (t.getdata() == ":")
                        table[rulename].setcmds(readrule(lex));
                    else if (t.getdata() == "?")
                        table[rulename].readruleoptions(lex);
                    else
                        throw runtime_error("Expected option line or rule definition");
                }
            } catch (runtime_error &error) {
                throw runtime_error(std::string(error.what()).append(" in ").append(systemname).append(" rule ").append(rulename));
            }
            t = lex.nexttoken();
        }
        thelsystems.emplace_back(systemname, activerule, table, startrule, expressions);
        if (t.iseof())
            return thelsystems;
    }
}
