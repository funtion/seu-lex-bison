#include "stdafx.h"

#ifndef _NFA_H
#define _NFA_H

#define MAX_NFA_NODE 2048

typedef int NFA_STATE_ID;

/* cost of nfa edge */
struct NFA_COST
{
	char c;
	bool epsilon;

};

struct NFA_EDGE
{
	NFA_COST cost;			/* char or epsilon */
	NFA_STATE_ID adjvex;	/* next nfa state */
	NFA_EDGE *next;

};

struct NFA_NODE
{
	NFA_EDGE *first_edge;
	
};

/* NFA_TABLE store all nfa states */
typedef NFA_NODE NFA_TABLE[MAX_NFA_NODE];

/*
  all we need know about a nfa is the initial and acceptting state
  others can be found in NFA_TABLE
*/
struct  NFA
{
	NFA_STATE_ID initial;
	NFA_STATE_ID accept;

	NFA(NFA_STATE_ID ini, NFA_STATE_ID acc)
	{
		initial = ini;
		accept = acc;
	}
};



/* initial a nfa table */
void ini_nfa_table(NFA_TABLE table);

/* creat a nfa state */
void creat_nfa_state(NFA_TABLE table, NFA_STATE_ID id);
/* add an epsilon edge between two state */
void nfa_epsilon_edge(NFA_TABLE table, NFA_STATE_ID s1, NFA_STATE_ID s2);
/* add a one-char edge between two states */
void nfa_c_edge(NFA_TABLE table, NFA_STATE_ID s1, NFA_STATE_ID s2, char c);

/* creat a epsilon nfa */
NFA creat_nfa(NFA_TABLE table, NFA_STATE_ID id);	
/* creat a one-char nfa */	
NFA creat_nfa_c(NFA_TABLE table, NFA_STATE_ID id, char c);	

/* concatenation */
NFA nfa_concat(NFA_TABLE table, NFA first, NFA second);
/* union */
NFA nfa_union(NFA_TABLE table, NFA_STATE_ID id, NFA first, NFA second);



/*
  for test
*/
void print_nfa_edge(NFA_EDGE *edge);
void print_nfa_state(NFA_TABLE table, NFA nfa);
void print_nfa_table(NFA_TABLE table, int col);



#endif