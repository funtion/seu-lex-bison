#pragma once
#include "YaccReader.h"
#include "LRBuilder.h"
#include "LALRBuilder.h"
class CompilerGenerater {
public:
	CompilerGenerater(const YaccReader& reader, const LRBuilder& builder,const LALRBuilder& lalrbuilder) 
		:reader(reader), lrbuilder(builder), lalrbuilder(lalrbuilder)
	{
	}

private:
	const YaccReader& reader;
	const LRBuilder& lrbuilder;
	const LALRBuilder& lalrbuilder;
public:

	int generateLR(const string& tplPath, const string& outPath);

	int generateTableHLR(const string& outPath);

	int generateLALR(const string& tplPath, const string& outPath);


};

