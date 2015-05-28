#include "stdafx.h"
#include "regex.h"
#include "nfa.h"

#ifndef _DFA_H
#define _DFA_H

#define CHAR_NUM 128
#define MAX_DFA_STATE 1024

typedef int DFA_STATE_ID;
typedef DFA_STATE_ID *DFA_STATE;
typedef DFA_STATE DFA_TABLE[MAX_DFA_STATE];
	
struct DFA
{
	DFA_STATE_ID id;
	DFA_TABLE tbl;
};

void dfa_state_ini(DFA_STATE state);
void dfa_ini(DFA &dfa);
void dfa_clear(DFA &dfa);

DFA_STATE_ID dfa_add_states(DFA &dfa);
void dfa_add_edge(DFA_TABLE dfa_tbl, DFA_STATE_ID begin, char c, DFA_STATE_ID end);
	
void nfa_to_dfa(NFA_TABLE nfa_tbl, NFA nfa, DFA &dfa);
void dfa_mini(DFA &dfa);
void dfa_write(DFA &dfa, FILE* file);
	
void print_dfa(DFA &dfa);





#endif