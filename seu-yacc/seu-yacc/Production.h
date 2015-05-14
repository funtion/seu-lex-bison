#include "stdafx.h"
#include "TOKEN.h"


#ifndef _PRODUCTION_H
#define _PRODUCTION_H

class Production {//存储所有产生式的向量
public: 
    Token left;
    vector<Token> right;
    string action;
};

#endif //_PRODUCTION_H