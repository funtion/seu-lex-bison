#include "stdafx.h"
#include "Production.h"


#ifndef _PRODUCTIONMANAGER_H
#define _PRODUCTIONMANAGER_H

class ProductionManager {
public: 
    
    /**
     * @param left
     * @param right
     */
    Production buildProduction(string left, vector<string> right);
    
    /**
     * @param procudtion
     */
    int getProductionID(const Production& procudtion);

	vector<Production> getProduction(NonterminalToken noterminal);

private: 
	map<Production,int> productions;
};

#endif //_PRODUCTIONMANAGER_H