#pragma once
class YaccReader
{
private:
	FILE* file;
public:
	YaccReader(FILE*);
	~YaccReader();
	int read();
private:
	string readUntil(const string& s);
public:
	// header code defined by user
	string userHeader;
	// code defined by user
	string userCode;
};

