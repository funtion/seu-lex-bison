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


TerminalToken TokenManager::buildToken(const string& name, const string& type, Associativity associativity, const string& precedence) {
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

NonterminalToken TokenManager::buildToken(const string& name) {
	if (tokens.find(name) == tokens.end()){
		int id = tokens.size();
		tokens[name] = id;
		NonterminalToken token;
		token.name = name;
		nonterminals[id] = token;
	}
	return nonterminals[tokens[name]];

}


bool TokenManager::isTerminal(const Token& token) {
	return isTerminal(token.name);
}

bool TokenManager::isTerminal(const string& token) {
	const int id = tokens[token];
	return terminals.find(id) == terminals.end();

}
