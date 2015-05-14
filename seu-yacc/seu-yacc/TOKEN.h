 


#ifndef _TOKEN_H
#define _TOKEN_H
enum Associativity
{
	LEFT,
	RIGHT
};
enum TokenType{
	TERMINAL,
	NO_TERMINAL
};
class Token {
public: 
    string name;
    string type;
	Associativity associativity;
    string precedence;
	TokenType tokentype;


	bool operator<(const Token& r) const
	{
		return name < r.name;
	}
};

#endif //_TOKEN_H