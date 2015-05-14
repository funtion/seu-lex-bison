#include "stdafx.h"
#include "YaccReader.h"


YaccReader::YaccReader(FILE* file,TokenManager& tokenManager, ProductionManager& productionManager) 
	:file(file), productionManager(productionManager), tokenManager(tokenManager)
{

}


YaccReader::~YaccReader()
{
	if (file != NULL){
		fclose(file);
		file = NULL;
	}
		
}


int YaccReader::read()
{
	readUntil("%{"); 
	userHeader = readUntil("%}");
	auto& tokenDefine = readUntil("%%");
	int c;
	while ((c = fgetc(file)) != EOF){
		userCode += (int)c;
	}
	return 0;
}


string YaccReader::readUntil(const string& s)
{
	string result = "";
	int c;
	while ((c=fgetc(file)) != EOF){
		result += (char)c;
		if (result.length() >= s.length()){
			const string& last = result.substr(result.length() - s.length(), s.length());
			if (last == s){
				result = result.substr(0, result.length() - s.length());
				break;
			}
		}
	}
	return result;
}
