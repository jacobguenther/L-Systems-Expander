#ifndef TOKEN_H
#define TOKEN_H
#include <string>
using std::string;

class Token {
   public:
    typedef enum { DATAT,
                   EOLT,
                   EOFT } Type;
    Token() {}
    Token(const string& _d, Type _t) : data(_d), mytype(_t) {}
    const string& getdata() const { return data; }
    bool isdata() const { return mytype == DATAT; }
    bool iseol() const { return mytype == EOLT; }
    bool iseof() const { return mytype == EOFT; }

   private:
    string data;
    Type mytype;
};

#endif
