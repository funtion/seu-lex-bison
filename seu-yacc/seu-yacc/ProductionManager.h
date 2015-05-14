#include "stdafx.h"
#include "Production.h"
#include "TokenManager.h"

#ifndef _PRODUCTIONMANAGER_H
#define _PRODUCTIONMANAGER_H

class ProductionManager {
public: 
	ProductionManager(TokenManager& tokenManager) :tokenManager(tokenManager) {

	}
    /**
     * @param left
     * @param right
     */
    Production buildProduction(string left, vector<string> right);
    
    /**
     * @param procudtion
     */
    int getProductionID(const Production& procudtion);
	/**
	* @param noterminal
	*/
	vector<Production> getProduction(NonterminalToken noterminal);

private: 
	map<Production,int> productions;
	TokenManager& tokenManager;
};

#endif //_PRODUCTIONMANAGER_H