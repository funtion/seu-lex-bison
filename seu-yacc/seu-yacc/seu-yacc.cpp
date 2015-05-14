// seu-yacc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "YaccReader.h"
int _tmain(int argc, char* argv[])
{
	if (argc != 2){
		cout << "usage: yacc file_name.y\n";
		return 0;
	}
	FILE* file = fopen(argv[1],"r");
	if (file == NULL){
		cout << "cannot open file " << argv[1] << endl;
		return 1;
	}
	YaccReader reader(file);


	return 0;
}



