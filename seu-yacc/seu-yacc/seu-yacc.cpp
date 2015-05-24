// seu-yacc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "YaccReader.h"
#include "TokenManager.h"
#include "ProductionManager.h"
#include "LRBuilder.h"
#include "CompilerGenerater.h"
int _tmain(int argc, char* argv[])
{
	if (argc != 2){
		cout << "usage: yacc file_name.y\n";
		//return 0;
	}
	FILE* file;
	int error;
	if((error = fopen_s(&file,"H:\\minic.y", "r"))){
		cout << "cannot open file ,error code "<< error << endl;
		return 1;
	}
	TokenManager tokenManager;
	ProductionManager productionManager(tokenManager);
	YaccReader reader(file, tokenManager, productionManager);
	reader.read();
	cout << "this is Header: "<<reader.tokenDefine;
	LRBuilder builder(tokenManager,productionManager);
	out << "[!!!!]start is set to e" << endl;
	builder.build("e");
	
	CompilerGenerater generater(reader,builder);
	generater.generateTableH("tab.h");
	generater.generate("result.tpl", "compiler.cpp");
	return 0;
}



