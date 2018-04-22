// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Lsystem.h"
#include "Cmd.h"
#include "Lexer.h"
#include "Parser.h"
#include "Rule.h"
#include "Token.h"
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

using std::ifstream;
using std::make_unique;
using std::runtime_error;
using std::string_view;
using std::vector;
using std::string;

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
            retval.push_back(make_unique<RotateCommand>(Parser(t.getdata()).parse()));
        } else if (t.getdata() == "right") {
            t = lex.nexttoken();
            assertdatatoken(t);
            retval.push_back(make_unique<RotateCommand>(Parser("-(" + t.getdata() + ")").parse()));
        } else if (t.getdata() == "|" || t.getdata() == "flip")
            retval.push_back(make_unique<FlipCommand>());
        else if (t.getdata() == "[" || t.getdata() == "push")
            retval.push_back(make_unique<PushCommand>());
        else if (t.getdata() == "]" || t.getdata() == "pop")
            retval.push_back(make_unique<PopCommand>());
        else {  //A rule
            bool rev = false;
            bool flip = false;
            string_view thisrule = t.getdata();
            while (thisrule[0] == '~' || thisrule[0] == '|') {
                if (thisrule[0] == '~') rev = true;
                if (thisrule[0] == '|') flip = true;
                thisrule.remove_prefix(1);
            }
            if (thisrule[thisrule.length() - 1] == '@') {
                thisrule.remove_suffix(1);
                auto localScale = lex.nexttoken();
                assertdatatoken(localScale);
                retval.push_back(make_unique<RuleCommand>(string(thisrule), rev, flip, Parser(localScale.getdata()).parse()));
            } else
                retval.push_back(make_unique<RuleCommand>(string(thisrule), rev, flip));
        }
        t = lex.nexttoken();
    }
    return retval;
}

Lsystem::Lsystem(string_view name, Lexer &lex) {
    _name = name;
    auto t = lex.nexttoken();
    while (t.isdata()) {
        if (t.getdata() == "inactive") {
            active = false;
            t = lex.nexttoken();
        } else
            throw runtime_error("Unrecognized option after system name: " + _name);
    }
    if (t.iseof())
        throw runtime_error("Unexpected end of file during " + _name);
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
            } else if (rulename == "system") {
                t = lex.nexttoken();
                assertdatatoken(t);
                if (t.getdata() != "?")
                    throw runtime_error("Expected '?' after keyword 'system'");
                readSystemOptions(lex);
            } else {
                if (startrule.empty())
                    startrule = rulename;
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
            throw runtime_error(std::string(error.what()).append(" in ").append(_name).append(" rule ").append(rulename));
        }
        t = lex.nexttoken();
    }
    if(!_drawStrategy)
        _drawStrategy = make_unique<LinesDrawStrategy>();
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
        //!!! Should be able to clean up this loop
        //I don't like reading the name, then calling the constructor
        //Some sort of builder pattern? I don't know
        Token t;
        do
            t = lex.nexttoken();
        while (t.iseol());  //get a non EOL token
        if (t.iseof()) {
            if (thelsystems.empty())
                throw runtime_error("No L systems in file " + configfilename);
            
            return thelsystems;
        }
        thelsystems.emplace_back(t.getdata(),lex);
    }
}

void Lsystem::readSystemOptions(Lexer &lex) {
    for (auto t = lex.nexttoken(); t.isdata(); t = lex.nexttoken()) {
        if (t.getdata() == "drawmethod") { //!!! Verify these all are appropriate and work
            t = lex.nexttoken();
            assertdatatoken(t);
            if (t.getdata() == "drop") {
                t = lex.nexttoken();
                assertdatatoken(t);
                auto dropAngleExpression = Parser(t.getdata()).parse();
                t = lex.nexttoken();
                assertdatatoken(t);
                auto dropDistanceExpression = Parser(t.getdata()).parse();
                _drawStrategy = make_unique<DropDrawStrategy>(move(dropAngleExpression),move(dropDistanceExpression));
            }
            else if (t.getdata() == "normal")
                _drawStrategy = make_unique<LinesDrawStrategy>();
//            else if (t.getdata() == "rectangle")
//                _drawMethod = RECT;
            else if (t.getdata() == "midpoint")
                _drawStrategy = make_unique<DropDrawStrategy>(Parser("0").parse(),Parser("1.0/2.0").parse());
            else
                throw std::runtime_error("Unexpected draw method " + t.getdata());
        } else if (t.getdata() == "info") {
            t = lex.nexttoken();
            assertdatatoken(t);
            _info = t.getdata();
        }
        else throw runtime_error("Unexpected system option: " + t.getdata());

    }
}

const std::string& Lsystem::getname() {
	return _name;
}

bool Lsystem::isactive() const {
	return active;
}
