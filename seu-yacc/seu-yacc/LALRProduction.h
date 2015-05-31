#ifndef _LALRPRODUCTION_H
#define _LALRPRODUCTION_H
#include "stdafx.h"
#include "TOKEN.h"


class LALRProduction {
public:
	int productionId;
	int pos;
	vector<int> lookAhead;
	/*bool operator<(const LRProduction& r) const {
		if (productionId != r.productionId)
		return productionId < r.productionId;
		if (pos != r.pos)
		return pos < r.pos;
		return lookAhead < r.lookAhead;
		}*/
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