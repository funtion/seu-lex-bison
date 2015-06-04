// seu-yacc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "YaccReader.h"
#include "TokenManager.h"
#include "ProductionManager.h"
#include "LRBuilder.h"
#include "LALRBuilder.h"
#include "CompilerGenerater.h"
int _tmain(int argc, char* argv[])
{
	if (argc != 2){
		cout << "usage: yacc file_name.y\n";
		//return 0;
	}
	FILE* file;
	int error;
	if((error = fopen_s(&file,"H:\\a.y", "r"))){
		cout << "cannot open file ,error code "<< error << endl;
		return 1;
	}
	TokenManager tokenManager;
	ProductionManager productionManager(tokenManager);
	YaccReader reader(file, tokenManager, productionManager);
	reader.read();
	LRBuilder lrbuilder(tokenManager, productionManager);
	LALRBuilder lalrbuilder(tokenManager,productionManager);

	cout << "[!!!!]start is set to e" << endl;
	//lalrbuilder.build("e");
	lrbuilder.build("E");


	//for (auto& i : lalrbuilder.allStatus())
	//{
	//	cout << "\n------lalrstatus-----------\n" << "--" << i.second;
	//}

	///*cout << "\n------   lalrtable   -----\n";
	//for (int i = 0; i < lalrbuilder.lrTable.size(); i++)
	//{
	//for (int j = 0; j < lalrbuilder.lrTable[i].size(); j++)
	//cout << lalrbuilder.lrTable[i][j]<< endl;
	//}

	//*/
	CompilerGenerater generater(reader, lrbuilder, lalrbuilder);
	generater.generateTableHLR("output/tab.h");
	generater.generateLR("result.tpl", "output/compiler.cpp");
	//generater.generateLALR("result.tpl", "output/compilerLALR.cpp");
	return 0;
}



