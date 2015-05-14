 


#ifndef _TOKEN_H
#define _TOKEN_H
enum Associativity
{
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
};

class NonterminalToken : public Token{
public:
	string type;
	Associativity associativity;
	string precedence;
};

class TerminalToken : public Token{

};

#endif //_TOKEN_H