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
	int currentState = 0;
	int lookAhaed = yylex();
	while(true){
		int act = action[currentState][lookAhaed];
		int tar = target[currentState][lookAhead];
		switch(act){
			case SHIFT:
				st.push(lookAhead);
				lookAhaed = yylex();
				if(lookAhead == EOF){
					printf("unexcepted EOF\n");
					return -1;
				}
				currentState = tar;
				break;
			case REDUCE:
				//TODO
				break;
			case GOTO:
				currentState = tar;
				break;
			case ACCPET:
				printf("success!\n");
				return 0;
			case ERROR:
			default:
			    printf("error\n");
				return -2;
		}
	}
	return 0;
}
/*user function*/
%s
