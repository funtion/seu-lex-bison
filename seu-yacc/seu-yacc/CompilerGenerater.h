#pragma once
#include "YaccReader.h"
#include "LRBuilder.h"
class CompilerGenerater {
public:
	CompilerGenerater(const YaccReader& reader, const LRBuilder& builder) 
		:reader(reader), builder(builder)
	{}
private:
	const YaccReader& reader;
	const LRBuilder& builder;
public:

	int generate(const string& tplPath, const string& outPath);
};

