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
	productionDefine = readUntil("%%");
	int c;
	while ((c = fgetc(file)) != EOF){ //����ַ����ļ�
		userCode += (char)c;
	}

	readtoken(tokenDefine);
	readproduct(productionDefine);
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
	}
	for (int i = 0; i <= n; i++)
	{
		string temp = buffer[i];
		int begin;
		string hed;
		begin = buffer[i].find('\t');                      //���Ʊ��Ϊ���ֵ� �ҳ�head���֡�
		hed = temp.substr(0, begin);
		temp = temp.substr(begin + 1, temp.length() - begin - 1);
		if (hed=="token")
		{
			TerminalToken ttoken;    //����һ���ս��
			char seg = ' ';
			while ((begin = temp.find(seg)) !=temp.npos)
			{			
				ttoken.name= temp.substr(0, begin);
				cout << "name:" << ttoken.name << endl;
				tokenManager.buildToken(ttoken.name,ttoken.type,ttoken.associativity,ttoken.precedence);
				ttoken.name = "";
				temp = temp.substr(begin + 1, temp.length() - begin - 1);
			}
			for (int i = 0; i < temp.length(); i++)        //��forѭ���������һ��token �����Զ���س������Ŀհ��У�
			{
				if (temp[i] != '\n')
					ttoken.name += temp[i];
			}
			cout << "name:" << ttoken.name<<endl;
			tokenManager.buildToken(ttoken.name, ttoken.type, ttoken.associativity, ttoken.precedence);
			ttoken.name = "";
		}
		else if (hed == "union")
		{
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
	/*Ϊttoken���type*/
	for (int i = 0; i < types.size(); i++)
	{
		string name=types[i].tokennames;
		for (int j = 0; j < unions.size(); j++)
		{
			if (types[i].typenames == unions[j].typenames)
			{
				string type = unions[j].type;
				tokenManager.setType(name, type);
				cout << "\n name:" << name << "type:" << type<<endl;
			}
		}	
	}


	/*����Ĵ�����������*/

	cout << "\n-----------The unions:---------------\n";
	for (int i = 0; i < unions.size(); i++)
		cout << unions[i].type << "\t\t" << unions[i].typenames << "\t" << endl;
	cout << "\n-----------The types:---------------\n";
	for (int i = 0; i < types.size(); i++)
		cout << types[i].tokennames << "\t" << types[i].typenames << "\t" << endl;
}
void YaccReader::readproduct(string productionDefine)
{
	string left="";
	vector<string>right;
	string action = "";
	string righttemp="";
	int flag = 1;//���һ������ʽ��״̬
	string buffer[100]; //������Ų���ʽ��ߵ����ݣ��������Լ�����start
	int n = 0;
	string temp = productionDefine;
	for (int i = 0; i < temp.length(); i++)
	{
		
		if ((flag == 1) && ((temp[i] >= 'a'&&temp[i] <= 'z') || (temp[i] >= 'A'&&temp[i] <= 'Z') || (temp[i] = '_')))//״̬1������ĸ�������
		{
			left += temp[i];
		}
		else if (flag == 2 && ((temp[i] >= 'a'&&temp[i] <= 'z') || (temp[i] >= 'A'&&temp[i] <= 'Z') || (temp[i]='_')))//״̬2������ĸ�����Ҳ�
		{
			righttemp += temp[i];
		}
		else if (flag==1&&temp[i] == ':')
		{
			flag = 2;   //����״̬2
		}
		else if (flag==2&&(temp[i] == ' '))//״̬2�����ո�
		{
			if (righttemp == "")
				continue;
			else
			{
				right.push_back(righttemp);
				righttemp = "";
			}
		}
		else if (flag ==2 && (temp[i] == '|'))//״̬2����|
		{
			productionManager.buildProduction(left, right, action);
			//cout << "\nleft:" << left << "----right:";
			for (int i = 0; i < right.size(); i++)
				right.pop_back();
			
		}
		else if (flag == 2 && (temp[i] == ';'))
		{
			productionManager.buildProduction(left, right, action);
			buffer[n] = left;
			n++;
			left = "";
			action = "";
			for (int i = 0; i < right.size(); i++)
				right.pop_back();
			flag = 1;
		}
		else if (flag == 2 && (temp[i] = '{'))
		{
			flag = 3;
		}
		else if (flag == 3 && (temp[i]) != '}')
		{
			action += temp[i];
		}
		else if (flag == 3 && (temp[i]) == '}')//����}�ص�״̬2
		{
			flag = 2;
		}

	}//end of read
	/*�������һ������ʽ�����ԷֺŽ�β��*/
	if (left != "")
	{
		productionManager.buildProduction(left, right, action);
		buffer[n] = left;
		left = "";
		action = "";
		for (int i = 0; i < right.size(); i++)
			right.pop_back();
	}

	start = buffer[0];
	/*for (int i = 0; i <= n; i++)
	{
	cout <<"  " <<buffer[i] << endl;
	}*/


	/*for (const auto& i: productionManager.all())
	{
		cout << i.first.left.name << i.first.right.size();
	}*/
	
}
