#include "stdafx.h"
#include "LRBuilder.h"


LRBuilder::LRBuilder(TokenManager& tokenManager, ProductionManager& productionManager):tokenManager(tokenManager),productionManager(productionManager) {
}




//build LR(1) status
int LRBuilder::build(const string& start) {
	auto& endToken = tokenManager.buildToken("_LR_END", "", LEFT, 0);
	string& startName = start + "_LR";
	auto& production = productionManager.buildProduction(startName, { start },"");
	int id = productionManager.getProductionID(production);//���Ҳ���ʽ û�������
	int endId = tokenManager.getTokenId(endToken.name);
	LRProduction lrProduction{ id, 0, endId}; //S' -> .s, $
	initFirst();
	startState = buildState({lrProduction});
	buildTable(start);
	return 0;
}


int LRBuilder::buildState(const vector<LRProduction> initProduction) {
	//build Closure
	LRState state{ initProduction, {} };
	while (true) {
		vector<LRProduction> newProduction;
		for (const auto& lrproduction : state.productions) {
			const auto& prooduction = productionManager.getProduction(lrproduction.productionId);
			if (lrproduction.pos < (int)prooduction.right.size()) { //dot is not at end
				const auto& nextToken = prooduction.right[lrproduction.pos];
				if (!tokenManager.isTerminal(nextToken)) {//.�����Ƿ��ս������ʼչ��
					const auto& nextProdutions = productionManager.getProductions((NonterminalToken&)nextToken);
					for (const auto& nextProdution : nextProdutions) {
						int id = productionManager.getProductionID(nextProdution);
						vector<int> tokenids;
						if (lrproduction.pos + 1 < (int)prooduction.right.size()) {
							int tid = tokenManager.getTokenId(prooduction.right[lrproduction.pos + 1].name);
							tokenids.push_back(tid);
						}
						tokenids.push_back(lrproduction.lookAhead);
						const auto& nextFirsts = getFirst(tokenids);
						for (auto& nextFirst : nextFirsts) {
							LRProduction nextLR{ id, 0, nextFirst};
							if (find(state.productions.begin(), state.productions.end(), nextLR) == state.productions.end()) {
								newProduction.push_back(nextLR);
							}
						}

					}
				}
			}
		}
		if (newProduction.size() == 0) {	//���������µĲ���ʽ�ˡ�
			break;
		}
		state.productions.insert(state.productions.end(), newProduction.begin(), newProduction.end());//�������ӵĲ���ʽ����״̬ĩβ
	}
	int sid = findState(state);
	if (sid != -1) {
		return sid;//?
	}
	int id = lrstatus.size();
	lrstatus[state] = id;
	// build GOTO
	map<Token, vector<LRProduction>> trans;
	for (const auto& lrproduction : state.productions) {
		const auto& prooduction = productionManager.getProduction(lrproduction.productionId);
		if (lrproduction.pos < (int)prooduction.right.size()) {
			const auto& nextToken = prooduction.right[lrproduction.pos];
			auto newLR = lrproduction;
			newLR.pos++;
			trans[nextToken].push_back(newLR);
		}
		else {//TODO X -> abc.

		}
	}
	for (const auto& tran : trans) {
		const auto& token = tran.first;
		const auto& productions = tran.second;
		state.action[token] = buildState(productions);
	}
	lrstatus_id[id] = state;
	return id;
}


int LRBuilder::initFirst() {
	const auto& allTerminal = tokenManager.allTerminal();
	for (auto& token : allTerminal) {
		first[token.first] = { token.first };//�ս��first���Ǳ���
		nullable[token.first] = token.second.name == "";
	}
	const auto& allProduction = productionManager.all();
	while (true) {
		bool changed = false;
		for (auto& prodictionWithID : allProduction) {
			const auto& production = prodictionWithID.first;
			//const int id = prodictionWithID.second;
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
					break;
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
		if (!changed) {
			break;
		}
	}

	return 0;
}


vector<int> LRBuilder::getFirst(const vector<int>& tokens) {
	vector<int> result;
	for (auto& token : tokens) {
		const auto& fi = first[token];
		result.insert(result.end(), fi.begin(), fi.end());
		if (!nullable[token]) {
			break;
		}
	}


	sort(result.begin(), result.end());
	result.erase(unique(result.begin(), result.end()), result.end());
	
	return result;
}


int LRBuilder::findState(const LRState& state) {
	for (const auto& s : lrstatus) {
		if (s.first.productions == state.productions)
			return s.second;
	}
	return -1;
}

void LRBuilder::buildTable(const string& start) {
	const int stateCnt = lrstatus.size();
	const int tokenCnt = tokenManager.size()-1;
	//initialize the table,set all state to error
	for (int i = 0; i < stateCnt; i++) {
		lrTable.push_back(vector<LRTableItem>(tokenCnt));
		auto& row = lrTable.back();
		for (int j = 0; j < tokenCnt; j++) {
			row[j].action = ERROR;
			row[j].target = -1;
		}
	}
	//set table items
	for (const auto& stateWithId : lrstatus_id) {
		auto& state = stateWithId.second;
		auto& id = stateWithId.first;
		auto& row = lrTable[id];
		for (auto& trans : state.action) {
			auto& token = trans.first;
			auto& target = trans.second;
			int tokenId = tokenManager.getTokenId(token.name);
			if (tokenManager.isTerminal(token)) {
				row[tokenId].action = LRAction::SHIFT;
			}
			else if(token.name != start){
				row[tokenId].action = LRAction::GOTO;
			}
			row[tokenId].target = target;
		}
		for (auto& lrProduction : state.productions) {
			const auto& production = productionManager.getProduction(lrProduction.productionId);
			if ((size_t)lrProduction.pos == production.right.size()) {// s -> abc.
				if (production.left.name == start + "'") {//S' -> s.
					row[lrProduction.lookAhead].action = LRAction::ACCEPT;
				}
				else {
					if (row[lrProduction.lookAhead].action == LRAction::ERROR) {
						row[lrProduction.lookAhead].action = LRAction::REDUCE;
						row[lrProduction.lookAhead].target = lrProduction.productionId;
					}
					else if (row[lrProduction.lookAhead].action == LRAction::SHIFT) {
						//TODO shift/reduce conflict
						auto last = production.right[0];
						for (auto& token : production.right) {
							if (!tokenManager.isTerminal(token)) {
								last = token;
							}
						}
						int lastId = tokenManager.getTokenId(last.name);
						int pri = tokenManager.comp(lastId,lrProduction.lookAhead);
						if (pri == 1) {
							row[lrProduction.lookAhead].action = LRAction::REDUCE;
							row[lrProduction.lookAhead].target = lrProduction.productionId;
						}
						else if (pri == 0) {//equal priority or not compireable
							TerminalToken& tlast = (TerminalToken&)last;
							if (tlast.associativity == LEFT) {
								row[lrProduction.lookAhead].action = LRAction::REDUCE;
								row[lrProduction.lookAhead].target = lrProduction.productionId;
							}
						}
					}
				}
			}
		}
	}

}
