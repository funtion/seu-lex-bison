#ifndef _LALRPRODUCTION_H
#define _LALRPRODUCTION_H
#include "stdafx.h"
#include "TOKEN.h"


class LALRProduction {
public:
	int productionId;
	int pos;
	vector<int> lookAhead;
	bool operator<(const LALRProduction& r) const {
		if (productionId != r.productionId)
			return productionId < r.productionId;
		if (pos != r.pos)
			return pos < r.pos;
		if (lookAhead.size()!=r.lookAhead.size())
			return lookAhead.size() < r.lookAhead.size();
		for (int i = 0; i < lookAhead.size();i++){
			if (lookAhead[i] != r.lookAhead[i])
				return lookAhead[i] < r.lookAhead[i];
		}
		return false;
	}
	void  setlookahead( vector<int>newahead){
		this->lookAhead.clear();
		for (int i = 0; i < newahead.size(); i++)
			this->lookAhead.push_back(newahead[i]);
	}
	bool operator==(const LALRProduction& r)const {
		return productionId == r.productionId && pos == r.pos && lookAhead == r.lookAhead;
	}
	bool LALRequal( LALRProduction& r)const{
		return  productionId == r.productionId && pos == r.pos;

	}
};

#endif //_LRPRODUCTION_H