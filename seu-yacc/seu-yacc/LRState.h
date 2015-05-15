#include "stdafx.h"
#include "LRProduction.h"
#include "TOKEN.h"

#ifndef _LRSTATE_H
#define _LRSTATE_H

class LRState {
public:    
	vector<LRProduction> productions;
    map<Token,int> action;
};

#endif //_LRSTATE_H