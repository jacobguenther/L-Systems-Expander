#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <utility>

class Token {
   public:
    enum Type { DATAT,
                   EOLT,
                   EOFT };
    Token()=default;
    Token(std::string  _d, Type _t) : data(std::move(_d)), mytype(_t) {}
    const std::string& getdata() const { return data; }
    bool isdata() const { return mytype == DATAT; }
    bool iseol() const { return mytype == EOLT; }
    bool iseof() const { return mytype == EOFT; }

   private:
    std::string data;
    Type mytype;
};

#endif
