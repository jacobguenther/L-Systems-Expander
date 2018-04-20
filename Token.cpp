#include "Token.h"

Token::Token(std::string _d, Type _t) :
		data(std::move(_d)), mytype(_t) {
}

const std::__1::string& Token::getdata() const {
	return data;
}

bool Token::isdata() const {
	return mytype == DATAT;
}

bool Token::iseol() const {
	return mytype == EOLT;
}

bool Token::iseof() const {
	return mytype == EOFT;
}
