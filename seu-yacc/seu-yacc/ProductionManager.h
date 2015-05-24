#include "stdafx.h"
#include "Production.h"
#include "TokenManager.h"

#ifndef _PRODUCTIONMANAGER_H
#define _PRODUCTIONMANAGER_H

class ProductionManager {
	friend class CompilerGenerater;
public: 
	ProductionManager(TokenManager& tokenManager) :tokenManager(tokenManager) {

	}
    /**
     * @param left
     * @param right
     */
    Production buildProduction(string left, vector<string> right,const string& action);
    
    /**
     * @param procudtion
     */
    int getProductionID(const Production& procudtion);
	/**
	* @param noterminal
	*/
	vector<Production> getProductions(const NonterminalToken& noterminal);

private: 
	map<Production,int> productionsID;
	map<int, Production> productions;
	TokenManager& tokenManager;
public:

	Production getProduction(int id) {
		return productions[id];
	}
	map<Production, int>& all() {
		return productionsID;
	}
};

#endif //_PRODUCTIONMANAGER_H