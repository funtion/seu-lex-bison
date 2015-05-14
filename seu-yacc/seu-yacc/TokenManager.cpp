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


NonterminalToken TokenManager::buildToken(string name, string type, Associativity associativity, string precedence){
	if (tokens.find(name) == tokens.end()){
		int id = tokens.size();
		tokens[name] = id;
		NonterminalToken token;
		token.name = name;
		token.type = type;
		token.associativity = associativity;
		token.precedence = precedence;
		nonterminals[id] = token;
	}
	return nonterminals[tokens[name]];
	
}

TerminalToken TokenManager::buildToken(string name) {
	if (tokens.find(name) == tokens.end()){
		int id = tokens.size();
		tokens[name] = id;
		TerminalToken token;
		token.name = name;
		terminals[id] = token;
	}
	return terminals[tokens[name]];

}
