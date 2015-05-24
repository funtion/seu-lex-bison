#include "stdafx.h"
#include "regex.h"

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

	NFA()
	{
		initial = -1;
		accept = -1;
	}

	NFA(NFA_STATE_ID ini, NFA_STATE_ID acc)
	{
		initial = ini;
		accept = acc;
	}
};



/* initial a nfa table */
void ini_nfa_table(NFA_TABLE table);

/* creat a nfa state */
NFA_STATE_ID creat_nfa_state(NFA_TABLE table, NFA_STATE_ID *id);
/* add an epsilon edge between two state */
void nfa_epsilon_edge(NFA_TABLE table, NFA_STATE_ID s1, NFA_STATE_ID s2);
/* add a one-char edge between two states */
void nfa_c_edge(NFA_TABLE table, NFA_STATE_ID s1, NFA_STATE_ID s2, char c);


/* creat a epsilon nfa */
NFA creat_nfa(NFA_TABLE table, NFA_STATE_ID *id);	
/* creat a one-char nfa */	
NFA creat_nfa_c(NFA_TABLE table, NFA_STATE_ID *id, char c);	


/* concatenation */
NFA nfa_concat(NFA_TABLE table, NFA first, NFA second);
/* union */
NFA nfa_union(NFA_TABLE table, NFA_STATE_ID *id, NFA first, NFA second);
/* kleen star */
NFA nfa_star(NFA_TABLE table, NFA_STATE_ID *id, NFA n);
/* postive clocure */
NFA nfa_pos_clo(NFA_TABLE table, NFA_STATE_ID *id, NFA n);
/* zero or one */
NFA nfa_zero_one(NFA_TABLE table, NFA_STATE_ID *id, NFA n);

/*          */
NFA regex_to_nfa(queue<RE> &regex, NFA_TABLE table, NFA_STATE_ID *id);
void foo(NFA_TABLE table, NFA_STATE_ID *id, re_type op, stack<NFA> &nfa);

NFA nfa_merge(NFA_TABLE table, NFA_STATE_ID *id, vector<NFA> v);


/* closure */

/* use a bool value to indicate whether it is taged */
typedef pair<NFA_STATE_ID, bool> idpair;
typedef map<NFA_STATE_ID, bool> idmap;

/* get the char set of a nfa */
set<char> nfa_char_set(NFA_TABLE table, NFA nfa);

idmap nfa_adj_e(NFA_TABLE table, NFA_STATE_ID id);
idmap nfa_e_closure(NFA_TABLE table, idmap ids);

NFA_STATE_ID nfa_adj_c(NFA_TABLE table, NFA_STATE_ID id, char c);
idmap nfa_cset(NFA_TABLE table, idmap ids, char c);


/*
  for test
*/

void print_nfa_edge(NFA_EDGE *edge);
void print_nfa_state(NFA_TABLE table, NFA nfa);
void print_nfa_table(NFA_TABLE table, int col);


#endif