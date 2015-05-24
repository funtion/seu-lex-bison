#include "stdafx.h"
#include "regex.h"
#include "nfa.h"

#ifndef _DFA_H
#define _DFA_H

typedef int DFA_STATE_ID;

struct DFA_EDGE
{
	char c;
	DFA_STATE_ID adjvex;
	
	DFA_EDGE(char ch, NFA_STATE_ID id) {
		c = ch;
		adjvex = id;
	}
};

typedef list<DFA_EDGE> DFA_NODE;

class DFA
{
private:
	vector<DFA_NODE> nodes;
	DFA_STATE_ID id;
	set<DFA_STATE_ID> accpet;
public:
	DFA();
	~DFA();
	void nfa_to_dfa(NFA_TABLE table, NFA nfa);
};




#endif