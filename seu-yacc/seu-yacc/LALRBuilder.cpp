#include "stdafx.h"
#include "LALRBuilder.h"
#include "LRBuilder.h"


LALRBuilder::LALRBuilder(TokenManager& tokenManager, ProductionManager& productionManager) :tokenManager(tokenManager), productionManager(productionManager) {
}




//build LALR(1) status
int LALRBuilder::build(const string& start) {
	string& startName = start + "_LR";
	auto& production = productionManager.buildProduction(startName, { start }, "");
	int id = productionManager.getProductionID(production);//查找产生式 没有则添加
	auto& endToken = tokenManager.buildToken("_LR_END", "", LEFT, 0);
	vector<int>endid;
	int endId = tokenManager.getTokenId(endToken.name);
	endid.push_back(endId);
	LALRProduction lalrProduction{ id, 0, endid }; //S' -> .s, $
	initFirst();
	startState = buildState({ lalrProduction });
	buildTable(start);
	return 0;
}


int LALRBuilder::buildState(const vector<LALRProduction> initProduction) {
	//build Closure
	LALRState state{ initProduction, {} };
	while (true) {
		vector<LALRProduction> newProduction;
		for (const auto& lalrproduction : state.productions) {
			const auto& prooduction = productionManager.getProduction(lalrproduction.productionId);
			if (lalrproduction.pos < (int)prooduction.right.size()) { //dot is not at end
				const auto& nextToken = prooduction.right[lalrproduction.pos];
				if (!tokenManager.isTerminal(nextToken)) {//.后面是非终结符，开始展开
					const auto& nextProdutions = productionManager.getProductions((NonterminalToken&)nextToken);
					for (const auto& nextProdution : nextProdutions) {
						int id = productionManager.getProductionID(nextProdution);
						vector<int> tokenids;
						if (lalrproduction.pos + 1 < (int)prooduction.right.size()) {
							int tid = tokenManager.getTokenId(prooduction.right[lalrproduction.pos + 1].name);
							tokenids.push_back(tid);
						}
						for (int i = 0; i < lalrproduction.lookAhead.size();i++)
						{
							tokenids.push_back(lalrproduction.lookAhead[i]);
						}
						const auto& nextFirsts = getFirst(tokenids);
						vector<int>nextlookahead;
						for (auto& nextFirst : nextFirsts) {
							nextlookahead.push_back(nextFirst);
						}
						LALRProduction nextLR{ id, 0, nextlookahead };
						if (find(state.productions.begin(), state.productions.end(), nextLR) == state.productions.end()) {
							newProduction.push_back(nextLR);
						}

					}
				}
			}
		}
		if (newProduction.size() == 0) {	//不再增加新的产生式了。
			break;
		}
		state.productions.insert(state.productions.end(), newProduction.begin(), newProduction.end());//将新增加的产生式插入状态末尾
	}
	int sid = findState(state);
	if (sid != -1) {
		return sid;//?
	}
	int id = lalrstatus.size();
	lalrstatus[state] = id;
	// build GOTO
	map<Token, vector<LALRProduction>> trans;
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
	lalrstatus_id[id] = state;
	return id;
}


int LALRBuilder::initFirst() {
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


vector<int> LALRBuilder::getFirst(const vector<int>& tokens) {
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


int LALRBuilder::findState(LALRState& state) {
	for (auto& s : lalrstatus) {
		int m = s.first.productions.size();
		int i = 0;
		LALRState Updates;
		for (; i < m; i++)
		{
			LALRProduction newproduciton;
			if (s.first.productions[i].LALRequal(state.productions[i]))//如果两个状态的产生式除lookahead外都相同
			{
				newproduciton.productionId = state.productions[i].productionId;
				newproduciton.pos = state.productions[i].pos;
				vector<int>newahead;
				newahead = s.first.productions[i].lookAhead;
				int l = state.productions[i].lookAhead.size();//lookahead 是vector形式
				for (int j = 0; j < l; j++)
				{
					newahead.push_back(state.productions[i].lookAhead[j]);//将两个lookahead合并
				}
				sort(newahead.begin(), newahead.end());
				newahead.erase(unique(newahead.begin(), newahead.end()), newahead.end());
				newproduciton.lookAhead = newahead;
				Updates.productions.push_back(newproduciton);
			}
			else
				break;
		}
		if (i == m)//证明两个lalr状态相同，更新状态
		{
			lalrstatus[Updates] = s.second;//？会不会替换？
			lalrstatus_id[s.second] = Updates;
			return s.second;
		}		
	}
	return -1;
}


void LALRBuilder::buildTable(const string& start) {
	
	const int stateCnt = lalrstatus.size();
	const int tokenCnt = tokenManager.size();
	//initialize the table,set all state to error
	for (int i = 0; i < stateCnt; i++) {
		lrTable.push_back(vector<LALRTableItem>(tokenCnt));
		auto& row = lrTable.back();
		for (int j = 0; j < stateCnt; j++) {
			row[j].action = ERROR;
			row[j].target = -1;
		}
	}
	//set table items
	for (const auto& stateWithId : lalrstatus_id) {
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
			else {
				row[tokenId].action = LRAction::GOTO;
			}
			row[tokenId].target = target;
		}
		for (auto& lrProduction : state.productions) {
			const auto& production = productionManager.getProduction(lrProduction.productionId);
			if ((size_t)lrProduction.pos == production.right.size()) {// s -> abc.
				if (production.left.name == start + "'") {//S' -> s.
					for (int i = 0; i < lrProduction.lookAhead.size();i++)
						row[lrProduction.lookAhead[i]].action = LRAction::ACCEPT;
				}
				else {
					for (int i = 0; i < lrProduction.lookAhead.size(); i++){
						if (row[lrProduction.lookAhead[i]].action == LRAction::ERROR) {
							row[lrProduction.lookAhead[i]].action = LRAction::REDUCE;
							row[lrProduction.lookAhead[i]].target = lrProduction.productionId;
						}
						else if (row[lrProduction.lookAhead[i]].action == LRAction::SHIFT) {
							//TODO shift/reduce conflict
							auto last = production.right[0];
							for (auto& token : production.right) {
								if (!tokenManager.isTerminal(token)) {
									last = token;
								}
							}
							int lastId = tokenManager.getTokenId(last.name);
							int pri = tokenManager.comp(lastId, lrProduction.lookAhead[i]);
							if (pri == 1) {
								row[lrProduction.lookAhead[i]].action = LRAction::REDUCE;
								row[lrProduction.lookAhead[i]].target = lrProduction.productionId;
							}
							else if (pri == 0) {//equal priority or not compireable
								TerminalToken& tlast = (TerminalToken&)last;
								if (tlast.associativity == LEFT) {
									row[lrProduction.lookAhead[i]].action = LRAction::REDUCE;
									row[lrProduction.lookAhead[i]].target = lrProduction.productionId;
								}
							}
						}
					}
				}
			}
		}
	}

}
