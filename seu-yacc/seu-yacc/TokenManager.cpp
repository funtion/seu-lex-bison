#include "stdafx.h"


#include "TokenManager.h"
#include "TOKEN.h"

/**
 * TokenManager implementation
 */


/**
 * @param token
 * @return int
 */


TerminalToken TokenManager::buildToken(const string& name, const string& type, Associativity associativity, const int& precedence) {
	if (tokens.find(name) == tokens.end()){
		int id = tokens.size();
		tokens[name] = id;
		TerminalToken token;
		token.name = name;
		token.type = type;
		token.associativity = associativity;
		token.precedence = precedence;
		terminals[id] = token;
	}
	return terminals[tokens[name]];
	
}

NonterminalToken TokenManager::buildToken(const string& name,const string& type) {
	if (tokens.find(name) == tokens.end()){
		int id = tokens.size();
		tokens[name] = id;
		NonterminalToken token;
		token.name = name;
		token.type = type;
		nonterminals[id] = token;
	}
	return nonterminals[tokens[name]];

}


bool TokenManager::isTerminal(const Token& token) {
	return isTerminal(token.name);
}

bool TokenManager::isTerminal(const string& token) {
	const int id = tokens[token];
	if (terminals[id].name==token)
	{
		return true;
	}
	return false;

}


int TokenManager::comp(int left, int right) {
	if (terminals.find(left) == terminals.end()) {
		return 0;
	}
	if (terminals.find(right) == terminals.end()) {
		return 0;
	}
	if (terminals[left].precedence < terminals[right].precedence) {
		return -1;
	}
	else if (terminals[left].precedence == terminals[right].precedence) {
		return 0;
	}
	return 1;
}
