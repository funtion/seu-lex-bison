#pragma once
#include "TokenManager.h"
#include "ProductionManager.h"
#include "LALRProduction.h"
#include "LALRState.h"
#include "LRBuilder.h"
#include "TOKEN.h"


struct LALRTableItem{
	LRAction action;
	int target;
};

class LALRBuilder {
	friend class CompilerGenerater;
public:
	LALRBuilder(TokenManager&, ProductionManager&);
private:
	TokenManager& tokenManager;
	ProductionManager& productionManager;
	map<LALRState, int> lalrstatus;
	map<int, LALRState > lalrstatus_id;
	map<int, vector<int>> first;
	map<int, bool> nullable;
public:
	int build(const string& start);

private:
	int buildState(vector<LALRProduction> initProduction);
	void buildTable(const string& start);
public:
	int startState;
	int initFirst();
	vector<int> getFirst(const vector<int>& tokens);
	int findState(LALRState& state);
	int findLALRState(const LALRState&state);
	vector<vector<LALRTableItem>> lrTable;
	inline map<LALRState, int> & allStatus() {
		return lalrstatus;
	}

};

