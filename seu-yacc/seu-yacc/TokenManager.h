#include "stdafx.h"
#include "TOKEN.h"

#ifndef _TOKENMANAGER_H
#define _TOKENMANAGER_H

enum TokenType{
	TERMINAL,
	NO_TERMINAL
};

class TokenManager {
public: 
    
    /**
     * @param token
     */
	TokenType getTokenType(Token token);
};

#endif //_TOKENMANAGER_H