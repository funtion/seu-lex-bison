#include "stdafx.h"
#include "TOKEN.h"

#ifndef _TOKENMANAGER_H
#define _TOKENMANAGER_H



class TokenManager {
public: 
	Token buildToken(string name, string type, Associativity associativity, string precedence, TokenType Tokentype);
private:
	map<Token, int> tokens;
};

#endif //_TOKENMANAGER_H