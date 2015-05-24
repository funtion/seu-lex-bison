#include "stdafx.h"
#include "LRProduction.h"
#include "TOKEN.h"

#ifndef _LRSTATE_H
#define _LRSTATE_H

class LRState {
public:    
	vector<LRProduction> productions;
    map<Token,int> action;
	bool operator<(const LRState& r)const {
		for (size_t i = 0; i < min(productions.size(), r.productions.size()); i++) {
			if (!(productions[i] == r.productions[i])) {
				return productions[i] < r.productions[i];
			}
		}
		return productions.size() < r.productions.size();
	}
};

#endif //_LRSTATE_H