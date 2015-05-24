#include "stdafx.h"
#include "regex.h"
#include "nfa.h"

#ifndef _DFA_H
#define _DFA_H

typedef unsigned int DFA_STATE_ID;

struct DFA_EDGE
{
	char c;
	DFA_STATE_ID adjvex;
	
	DFA_EDGE(char ch, NFA_STATE_ID id) {
		c = ch;
		adjvex = id;
	}
};

typedef list<DFA_EDGE> DFA_STATE;

class DFA
{
private:
	vector<DFA_STATE> states;
	DFA_STATE_ID id;
	set<DFA_STATE_ID> accept;
	
	void add_states();
	void add_edge(DFA_STATE_ID begin, char c, DFA_STATE_ID end);
	
public:
	DFA();
	~DFA();

	void nfa_to_dfa(NFA_TABLE table, NFA nfa);
	void dfa_mini();
	void dfa_write(FILE* file);
	void print_dfa();
};




#endif