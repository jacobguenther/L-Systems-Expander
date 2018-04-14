#ifndef TOKEN_H
#define TOKEN_H
#include <string>

class Token {
   public:
    enum Type { DATAT,
                   EOLT,
                   EOFT };
    Token()=default;
    Token(const std::string& _d, Type _t) : data(_d), mytype(_t) {}
    const std::string& getdata() const { return data; }
    bool isdata() const { return mytype == DATAT; }
    bool iseol() const { return mytype == EOLT; }
    bool iseof() const { return mytype == EOFT; }

   private:
    std::string data;
    Type mytype;
};

#endif
