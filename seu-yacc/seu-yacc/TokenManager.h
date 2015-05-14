#include "stdafx.h"
#include "TOKEN.h"

#ifndef _TOKENMANAGER_H
#define _TOKENMANAGER_H



class TokenManager {
public: 
	NonterminalToken buildToken(string name, string type, Associativity associativity, string precedence);
	TerminalToken buildToken(string name);
private:
	map<string, int> tokens;
	map<int, TerminalToken> terminals;
	map<int, NonterminalToken> nonterminals;
};

#endif //_TOKENMANAGER_H