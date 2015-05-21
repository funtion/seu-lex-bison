%s 
/*user defined header*/
%s 
/*lr table*/
#include <stack>
using namespace std;
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

%s /*user function*/
