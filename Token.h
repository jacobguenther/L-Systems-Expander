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
	Token(std::string _d, Type _t);
	const std::string& getdata() const;
	bool isdata() const;
	bool iseol() const;
	bool iseof() const;

   private:
    std::string data;
    Type mytype=EOFT;
};
#endif
