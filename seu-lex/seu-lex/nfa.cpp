#include "stdafx.h"
#include "nfa.h"

/* initial a nfa table */
void ini_nfa_table(NFA_TABLE table)
{
	for (int i = 0; i <MAX_NFA_NODE; i++)
		table[i].first_edge = NULL;
}


/* creat a nfa state */
void creat_nfa_state(NFA_TABLE table, NFA_STATE_ID id)
{
	// todo
}

/* add an epsilon edge between two state */
void nfa_epsilon_edge(NFA_TABLE table, NFA_STATE_ID s1, NFA_STATE_ID s2)
{

	NFA_EDGE *new_edge = (NFA_EDGE *) malloc(sizeof(NFA_EDGE));
	new_edge->cost.epsilon = true;
	new_edge->adjvex = s2;
	new_edge->next = table[s1].first_edge;

	table[s1].first_edge = new_edge;
}
/* add a one-char edge between two states */
void nfa_c_edge(NFA_TABLE table, NFA_STATE_ID s1, NFA_STATE_ID s2, char c)
{
	NFA_EDGE *new_edge = (NFA_EDGE *) malloc(sizeof(NFA_EDGE));
	new_edge->cost.epsilon = false;
	new_edge->cost.c = c;
	new_edge->adjvex = s2;
	new_edge->next = table[s1].first_edge;

	table[s1].first_edge = new_edge;
}



/* creat a epsilon nfa */
NFA creat_nfa(NFA_TABLE table, NFA_STATE_ID id)
{
	creat_nfa_state(table, id);
	creat_nfa_state(table, id+1);
	nfa_epsilon_edge(table, id, id+1);
	return NFA(id, id+1);
}

/* creat a one-char nfa */	
NFA creat_nfa_c(NFA_TABLE table, NFA_STATE_ID id, char c)
{
	creat_nfa_state(table, id);
	creat_nfa_state(table, id+1);
	nfa_c_edge(table, id, id+1, c);
	return NFA(id, id+1);
}

/* concatenation */
NFA nfa_concat(NFA_TABLE table, NFA first, NFA second)
{
	nfa_epsilon_edge(table, first.accept, second.initial);
	return NFA(first.initial, second.accept);
}

/* union */
NFA nfa_union(NFA_TABLE table, NFA_STATE_ID id, NFA first, NFA second)
{
	NFA_STATE_ID ini = id;
	NFA_STATE_ID acc = id+1;
	creat_nfa_state(table, ini);	/* initial state */
	creat_nfa_state(table, acc);	/* accept state */

	nfa_epsilon_edge(table, ini, first.initial);
	nfa_epsilon_edge(table, ini, second.initial);
	nfa_epsilon_edge(table, first.accept, acc);
	nfa_epsilon_edge(table, second.accept, acc);

	return NFA(ini, acc);
}


/*
  for test
*/
void print_nfa_edge(NFA_EDGE *edge)
{
	if (edge->cost.epsilon)
		printf("[ epsilon, ");
	else
		printf("[ %c, ", edge->cost.c);

	printf("%d] ", edge->adjvex);
}

void print_nfa_state(NFA_TABLE table, NFA_STATE_ID s)
{
	NFA_EDGE *e = table[s].first_edge;
	for (; e != NULL; e = e->next)
		print_nfa_edge(e);

}

void print_nfa_table(NFA_TABLE table, int col)
{
	NFA_STATE_ID s;
	for (s = 0; s < col; s++) {
		printf("%d:  ", s);
		print_nfa_state(table, s);
		printf("\n");
	}
}
