 

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
Production ProductionManager::buildProduction(string left, vector<string> right) {
	Production production;
	production.left = tokenManager.buildToken(left);
	for (const auto& r : right) {
		production.right.push_back(tokenManager.buildToken(r));
	}
	if (productions.find(production) == productions.end()) {
		int id = productions.size();
		productions[production] = id;
	}
	return production;
}

/**
 * @param procudtion
 * @return int
 */
int ProductionManager::getProductionID(const Production& procudtion) {
	return productions[procudtion];
}
/**
* @param noterminal
* @return vector<Production>
*/
vector<Production> ProductionManager::getProduction(NonterminalToken noterminal) {

}