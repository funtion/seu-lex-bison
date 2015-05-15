#include "stdafx.h"
#include "LRBuilder.h"


LRBuilder::LRBuilder(TokenManager& tokenManager, ProductionManager& productionManager):tokenManager(tokenManager),productionManager(productionManager) {
}




//build LR(1) status
int LRBuilder::build() {
	string& startName = startToken.name + "'";
	auto& production = productionManager.buildProduction(startName, { startToken.name });
	int id = productionManager.getProductionID(production);
	auto& endToken = tokenManager.buildToken("$", "", LEFT, "");
	LRProduction lrProduction{ id, 0, { endToken } }; //S' -> .s, $
	startState = buildState({lrProduction});
	return 0;
}


LRState LRBuilder::buildState(const vector<LRProduction> initProduction) {
	//build Closure
	LRState state{ initProduction, {} };
	while (true) {
		bool newProduction = false;
		for (const auto& lrproduction : state.productions) {
			const auto& prooduction = productionManager.getProduction(lrproduction.productionId);
			if (lrproduction.pos < (int)prooduction.right.size()) { //dot is not at end
				const auto& nextToken = prooduction.right[lrproduction.pos];
				if (!tokenManager.isTerminal(nextToken)) {
					const auto& nextProdutions = productionManager.getProductions((NonterminalToken&)nextToken);
					for (const auto& nextProdution : nextProdutions) {
						int id = productionManager.getProductionID(nextProdution);
						LRProduction nextLR{ id, 0, {} };//TODO first
					}
				}
			}
		}
	}


	return LRState();
}
