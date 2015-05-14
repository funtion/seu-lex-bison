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
	FILE* file;
	int error;
	if(error = fopen_s(&file,argv[1], "r")){
		cout << "cannot open file " << argv[1]<<"error code "<< error << endl;
		return 1;
	}
	YaccReader reader(file);
	reader.read();
	return 0;
}



