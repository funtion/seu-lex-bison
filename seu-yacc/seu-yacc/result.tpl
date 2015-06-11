/*user defined header*/
%s 
/*lr table*/
%s
/*productions*/
%s
/*yyparser*/
#include <stack>
#include <cstdio>
#include "tab.h"
using std::stack;
#define SHIFT 0
#define REDUCE 1
#define GOTO 2
#define ACCPET 3
#define ERROR  4
int yyParser(){
	stack<int> st;
	st.push(0);
	int tk = yylex();
	while(true){
		int s = st.top();
		int act = action[s][tk];
		if(act == REDUCE){
			int* production = productions[target[s][tk]];
			int left = production[1];
			int rightNum = production[2];
			//token and state are included
			for(int i=0;i<rightNum*2;i++){
				st.pop();
			}
			s = st.top();
			st.push(left);
			st.push(target[s][left]);
			//TODO add action
		}else if(act == SHIFT){
			st.push(tk);
			st.push(target[s][tk]);
			tk = yylex();
			if(tk == EOF){
				cout<<"Error:unexceped End Of File!!\n";
				return -1;
			}
		}else if(act == SUCCESS){
			cout<<"success!!\n";
			return 0;
		}else{
			cout<<"error!!\n";
			return -1;
		}
	}
	return 0;
}
/*user function*/
%s
