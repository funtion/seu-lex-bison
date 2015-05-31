#include "stdafx.h"
#include "LALRProduction.h"
#include "TOKEN.h"

#ifndef _LALRSTATE_H
#define _LALRSTATE_H

class LALRState {
public:
	vector<LALRProduction> productions;
	map<Token, int> action;
	bool operator<(const LALRState& r)const {
		for (size_t i = 0; i < min(productions.size(), r.productions.size()); i++) {
			if (!(productions[i] == r.productions[i])) {
				return productions[i] < r.productions[i];
			}
		}
		return productions.size() < r.productions.size();
	}
	
	bool addLALR(LALRState& r){

		for (size_t i = 0; i < min(productions.size(), r.productions.size()); i++)
		{
			if (productions[i].LALRequal(r.productions[i]))
				productions.push_back(r.productions[i]);
		}
	}
};

#endif //_LRSTATE_H