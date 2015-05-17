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
	LRState buildState(vector<LRProduction> initProduction);
public:
	LRState startState;
	int initFirst();
};

