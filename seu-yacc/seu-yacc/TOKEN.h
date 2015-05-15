 


#ifndef _TOKEN_H
#define _TOKEN_H
enum Assiciativity
{
	LEFT,
	RIGHT
};
class Token {
public: 
    string name;
    string type;
    Assiciativity associativity;
    string precedence;
};

#endif //_TOKEN_H