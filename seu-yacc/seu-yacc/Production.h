#include "stdafx.h"
#include "TOKEN.h"


#ifndef _PRODUCTION_H
#define _PRODUCTION_H

class Production {
public: 
    Token left;
    vector<Token> right;
    string action;
	bool operator<(const Production& r)const {
		if (left == r.left) {
			for (int i = 0; i < (int)min(right.size(), r.right.size()); i++) {
				if (! (right[i] == r.right[i])) {
					return right[i] < r.right[i];
				}
			}
			return right.size() < r.right.size();
		}
		return left < r.left;
	}
};

#endif //_PRODUCTION_H