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


int LRBuilder::initFirst() {
	const auto& allTerminal = tokenManager.allTerminal();
	for (auto& token : allTerminal) {
		first[token.first] = { token.first };
		nullable[token.first] = token.second.name == "";
	}
	const auto& allProduction = productionManager.all();
	while (true) {
		bool changed = false;
		for (auto& prodictionWithID : allProduction) {
			const auto& production = prodictionWithID.first;
			const int id = prodictionWithID.second;
			int k = production.right.size();
			bool canNull = true;
			for (int i = 0; i < k; i++) {
				int rid = tokenManager.getTokenId(production.right[i].name);
				if (!nullable[rid]) {
					canNull = false;
					break;
				}
			}
			int lid = tokenManager.getTokenId(production.left.name);
			if (canNull) {
				nullable[lid] = true;
				changed = true;
			}
			bool allNull = true;
			auto& fx = first[lid];
			for (int i = 0; i < k; i++) {
				int rid = tokenManager.getTokenId(production.right[i].name);
				if (!nullable[rid]) {
					allNull = false;
					const auto& fy = first[rid];
					for (auto& f : fy) {
						if (find(fx.begin(), fx.end(), f) == fx.end()) {
							changed = true;
							fx.push_back(f);
						}
					}
				}
			}
			//s -> "",add "" to first s
			if (allNull) {
				int nid = tokenManager.getTokenId("");
				if (find(fx.begin(), fx.end(), nid) == fx.end()) {
					changed = true;
					fx.push_back(nid);
				}
			}
		}
		if (changed) {
			break;
		}
	}

	return 0;
}
