 

#include "stdafx.h"
#include "ProductionManager.h"

/**
 * ProductionManager implementation
 */


/**
 * @param left
 * @param right
 * @return Procudtion
 */
Production ProductionManager::buildProduction(string left, vector<string> right,const string& action) {
	Production production;
	production.left = tokenManager.buildToken(left);
	for (const auto& r : right) {
		production.right.push_back(tokenManager.buildToken(r));
	}
	production.action = action;
	if (productionsID.find(production) == productionsID.end()) {
		int id = productions.size();
		productionsID[production] = id;
		productions[id] = production;
	}
	return production;
}

/**
 * @param procudtion
 * @return int
 */
int ProductionManager::getProductionID(const Production& procudtion) {
	return productionsID[procudtion];
}
/**
* @param noterminal
* @return vector<Production>
*/
vector<Production> ProductionManager::getProductions(const NonterminalToken& noterminal) {
	//TODO all production of a no terminal
	return{};

}