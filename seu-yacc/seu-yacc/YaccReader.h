#pragma once
#include "ProductionManager.h"
#include "TokenManager.h"
class YaccReader
{
public:
	YaccReader(FILE*, TokenManager& tokenManager, ProductionManager& productionManager);
	~YaccReader();
	int read();
private:
	string readUntil(const string& s);
public:
	// header code defined by user
	string userHeader;
	// code defined by user
	string userCode;
	TokenManager& tokenManager;
	ProductionManager& productionManager;
private:
	FILE* file;
};

