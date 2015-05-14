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


Token TokenManager::buildToken(string name, string type, Associativity associativity, string precedence,TokenType Tokentype)
{
	Token token{ name, type, associativity, precedence ,Tokentype};
	if (tokens.find(token) == tokens.end()){
		int id = tokens.size();
		tokens[token] = id;
	}
	return token;
}
