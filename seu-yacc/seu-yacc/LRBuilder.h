#pragma once
#include "TokenManager.h"
#include "ProductionManager.h"
#include "LRProduction.h"
#include "LRState.h"
#include "TOKEN.h"

enum LRAction {
	SHIFT,
	REDUCE,
	GOTO,
	ACCEPT,
	ERROR
};

struct LRTableItem{
	LRAction action;
	int target;
};

class LRBuilder {
	friend class CompilerGenerater;
public:
	LRBuilder(TokenManager&, ProductionManager&);
private:
	TokenManager& tokenManager;
	ProductionManager& productionManager;
	map<LRState, int> lrstatus;
	map<int, LRState > lrstatus_id;
	map<int, vector<int>> first;
	map<int, bool> nullable;
public:
	int build(const string& start);
	
private:
	int buildState(vector<LRProduction> initProduction);
	void buildTable(const string& start);
public:
	int startState;
	int initFirst();
	vector<int> getFirst(const vector<int>& tokens);
	int findState(const LRState& state);
	vector<vector<LRTableItem>> lrTable;
	
};

