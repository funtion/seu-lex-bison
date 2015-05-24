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
	void readtoken(string tokenDefine);
public:
	// header code defined by user
	string userHeader;
	//the taken
	string tokenDefine;
	// code defined by user
	string userCode;
	TokenManager& tokenManager;
	ProductionManager& productionManager;
private:
	FILE* file;
	struct Union
	{
		string type;
		string typenames;
	};
	struct Type
	{
		string typenames;
		string tokennames;
	};
	vector<Union>unions;
	vector<Type>types;
	vector<Token>tokens;//��Ŷ�����Token
	
public:
	NonterminalToken startToken;
	
};

