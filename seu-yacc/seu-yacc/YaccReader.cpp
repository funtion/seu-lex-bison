#include "stdafx.h"
#include "YaccReader.h"
#include "TOKEN.h"

using namespace std;
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
	tokenDefine = readUntil("%%");
	int c;
	while ((c = fgetc(file)) != EOF){ //����ַ����ļ�
		userCode += (char)c;
	}

	readtoken(tokenDefine);
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
			if (last==s){
				result = result.substr(0, result.length() - s.length());
				break;
			}
		}
	}
	return result;
}

void YaccReader::readtoken(string tokenDefine)
{
	string buffer[100];
	int n = 0;
	int count;
	while ((count = tokenDefine.find("%", 1)) != tokenDefine.npos)//find�����ҵ��ɹ����ء�%���ı�ǣ�ʧ�ܷ���npos
	{

		buffer[n] = tokenDefine.substr(1, count - 1);
		tokenDefine = tokenDefine.substr(count, tokenDefine.length() - count);
		if (buffer[n] =="\n")                               //���Կհ��С�
			continue;
		else
			n++;
		
	}
	buffer[n] = tokenDefine.substr(1, tokenDefine.length() - 1);

	for (int o = 0; o <= n; o++)               //����/*****/��
	{
		count = buffer[o].find("/*");
		buffer[o] = buffer[o].substr(0, count);
	//	cout << buffer[o] << "\n--------------------------\n";
	}


	for (int i = 0; i <= n; i++)
	{
		string temp = buffer[i];

		int begin;
		string hed;
		begin = buffer[i].find('\t');                      //���Ʊ��Ϊ���ֵ� �ҳ�head���֡�
		hed = temp.substr(0, begin);
		temp = temp.substr(begin + 1, temp.length() - begin - 1);
		//cout << "hed:  " << hed<<endl;
		
		if (hed=="token")
		{
			char seg = ' ';
			while ((begin = temp.find(seg)) !=temp.npos)
			{
				Token token;
				token.name= temp.substr(0, begin);
				tokens.push_back(token);
				temp = temp.substr(begin + 1, temp.length() - begin - 1);
			}
			Token token;
			//token.name = temp.substr(0, temp.length()-1);
			for (int i = 0; i < temp.length(); i++)        //��forѭ���������һ��token �����Զ���س������Ŀհ��У�
			{
				if (temp[i] != '\n')
					token.name += temp[i];
			}
			tokens.push_back(token);
			for (int i = 0; i < tokens.size(); i++)
				cout << tokens[i].name << " ";
		}
		else if (hed == "union")
		{
			cout <<"this is temp:"<< temp<<endl;
			while ((begin = temp.find("\n")) != temp.npos)             //�ҳ�union�е�ÿһ��
			{
				string line;
				line=temp.substr(0, begin);
				temp = temp.substr(begin + 1, temp.length() - begin - 1);
				//cout << line<<"\n-------------\n";
				if (line=="{")                 //���Դ�����
					continue;
				if (line == "\t}"||line=="}")
					break;
				else
				{
					Union uni;
					int t=line.find("\t", 1);
					string type;
					for (int i = 0; i <t; i++)
					{
						//if ((line[i] >= 'a'&&line[i] <= 'z') || (line[i] >= 'A'&&line[i] <= 'Z')||(line[i]=='_'|'*'))  //���Կո���Ʊ��
						if ((line[i]!='\t')&&(line[i]!=' '))
							type += line[i];
					}
					uni.type = type;
					line = line.substr(t+1, line.length() - t-1);
					t = line.find(";", 1);
					string typenames;
					for (int i = 0; i < t; i++)
					{
						if ((line[i] >= 'a'&&line[i] <= 'z') || (line[i] >= 'A'&&line[i] <= 'Z'))  //���Կո���Ʊ��
							typenames += line[i];
					}
					uni.typenames = typenames;
					if (uni.type!="")
						unions.push_back(uni);
				}
			}
		} 
		else if (hed == "type")
		{
			begin = temp.find("\t");
			string typn;
			typn= temp.substr(1,begin-2);        //��1��begin-2 Ϊ���������˵ġ�<>��
			temp = temp.substr(begin + 1, temp.length() - begin - 1);
			int t;
			while ((t = temp.find(" ", 1)) != temp.npos)
			{
				Type type;
				type.typenames = typn;
				type.tokennames = temp.substr(0, t);
				types.push_back(type);
				temp = temp.substr(t + 1, temp.length() - t - 1);
			}
			Type type;
			type.typenames = typn;
			//type.tokennames = temp.substr(0, temp.length()-1);   //�������һ��type  -1 Ϊ��ȥ�� \n
			for (int i = 0; i < temp.length(); i++)
			{
				if (temp[i]!='\n')
				{
					type.tokennames += temp[i];
				}
			}
			types.push_back(type);

		}
		else if (hed == "start")
		{

		}
	} //end of for


	/*����Ĵ�����������*/
	cout << "\n-----------The unions:---------------\n";
	for (int i = 0; i < unions.size(); i++)
		cout << unions[i].type << "\t\t" << unions[i].typenames << "\t" << endl;
	cout << "\n-----------The types:---------------\n";
	for (int i = 0; i < types.size(); i++)
		cout << types[i].tokennames << "\t" << types[i].typenames << "\t" << endl;
	//int i = 0;
	//int begin;
	//while ((begin = buffer[i].find("token")) != buffer[i].npos)
	//{
	//	cout << begin;
	//	string temp = buffer[i];
	//	for (int j = begin; j < temp.length(); j++)
	//	{
	//		if (temp[j] == ' ')//���Կո�
	//			continue;
	//		else
	//		{

	//		}
	//	}
	//}
}
void producttoken()
{

}