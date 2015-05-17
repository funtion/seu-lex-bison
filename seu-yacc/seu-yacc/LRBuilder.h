#pragma once
#include "TokenManager.h"
#include "ProductionManager.h"
#include "LRProduction.h"
#include "LRState.h"
#include "TOKEN.h"

class LRBuilder {
public:
	LRBuilder(TokenManager&, ProductionManager&);
private:
	TokenManager& tokenManager;
	ProductionManager& productionManager;
	NonterminalToken startToken;
	map<LRState, int> lrstatus;
	map<int, vector<int>> first;
	map<int, bool> nullable;
public:
	int build();
	
private:
	int buildState(vector<LRProduction> initProduction);
public:
	int startState;
	int initFirst();
	vector<int> getFirst(const vector<int>& tokens);
	int findState(const LRState& state);
};

