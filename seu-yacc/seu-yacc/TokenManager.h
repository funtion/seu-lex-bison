#include "stdafx.h"
#include "TOKEN.h"

#ifndef _TOKENMANAGER_H
#define _TOKENMANAGER_H



class TokenManager {
public: 
	TerminalToken buildToken(const string& name, const string& type, Associativity associativity, const string& precedence);
	NonterminalToken buildToken(const string& name);
private:
	map<string, int> tokens;
	map<int, TerminalToken> terminals;
	map<int, NonterminalToken> nonterminals;
public:
	bool isTerminal(const Token& token);
	bool isTerminal(const string& token);
};

#endif //_TOKENMANAGER_H