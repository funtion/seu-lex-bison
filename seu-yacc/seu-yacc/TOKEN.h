 


#ifndef _TOKEN_H
#define _TOKEN_H
enum Associativity
{
	NONE,
	LEFT,
	RIGHT
};

class Token {
public: 
    string name;
    
	bool operator<(const Token& r) const
	{
		return name < r.name;
	}
	bool operator==(const Token&r)const {
		return name == r.name;
	}
};

class TerminalToken : public Token{
public:
	string type;
	Associativity associativity;
	string precedence;
};

class NonterminalToken : public Token {

};

#endif //_TOKEN_H