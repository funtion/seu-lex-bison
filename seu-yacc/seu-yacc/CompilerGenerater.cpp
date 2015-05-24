#include "stdafx.h"
#include "CompilerGenerater.h"
int CompilerGenerater::generate(const string& tplPath, const string& outPath) {
	FILE* tplFile = fopen(tplPath.c_str(), "r");
	FILE* outFile = fopen(outPath.c_str(), "w");
	const size_t MAXN = 8192;
	char* tpl = new char[MAXN];
	fread(tpl, sizeof(char), MAXN, tplFile);
	stringstream lrTable;

	lrTable << "int action[][" << builder.lrTable[0].size() << "] = {";
	for (size_t i = 0; i < builder.lrTable.size(); i++) {
		lrTable << "{";
		for (size_t j = 0; j < builder.lrTable[i].size(); j++) {
			lrTable << builder.lrTable[i][j].action << " ,";
		}
		lrTable << "},\n";
	}
	lrTable << "};\n";
	lrTable << "int target[][" << builder.lrTable[0].size() << "] = {\n";
	for (int i = 0; i < (int)builder.lrTable.size(); i++) {
		lrTable << "{";
		for (int j = 0; j < (int)builder.lrTable[i].size(); j++) {
			lrTable << builder.lrTable[i][j].target << " ,";
		}
		lrTable << "},\n";
	}
	lrTable << "};\n";
	string lrTableString;
	lrTable >> lrTableString;
	stringstream productions;
	auto &productionManager = builder.productionManager;
	productions << "int productions[]["<<10<<"]={\n";
	for (auto& pro : productionManager.productions) {
		//[id,left,num,right....]
		productions << "{" << pro.first<<","<<pro.second.left.name << "," << pro.second.right.size() << ",";
		for (const auto& r : pro.second.right) {
			productions << r.name << ",";
		}
		productions << "}\n";
	}
	productions << "};\n";
	string productionString;
	productions >> productionString;
	fprintf(outFile, tpl, reader.userHeader.c_str(), lrTableString.c_str(), productionString.c_str(), reader.userCode.c_str());
	delete[] tpl;
	fclose(tplFile);
	fclose(outFile);
	return 0;
}
int CompilerGenerater::generateTableH(const string& outPath) {
	FILE* outFile = fopen(outPath.c_str(), "w");
	stringstream stream;
	stream << "#ifndef __TABLE_H";
	stream << "#define __TABLE_H";
	for (auto& token : reader.tokenManager.tokens) {
		stream << "#define " << token.first << " " << token.second << '\n';
	}
	stream << "#endif";
	string s;
	stream >> s;
	fprintf(outFile, "%s", s.c_str());
	fclose(outFile);
	return 0;
}
