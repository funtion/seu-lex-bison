#include "stdafx.h"
#include "nfa.h"

/* initial a nfa table */
void ini_nfa_table(NFA_TABLE table)
{
	for (int i = 0; i <MAX_NFA_NODE; i++)
		table[i].first_edge = NULL;
}


/* creat a nfa state */
NFA_STATE_ID creat_nfa_state(NFA_TABLE table, NFA_STATE_ID *id)
{
	return (*id)++;
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
NFA creat_nfa(NFA_TABLE table, NFA_STATE_ID *id)
{
	NFA_STATE_ID ini = creat_nfa_state(table, id);
	NFA_STATE_ID acc = creat_nfa_state(table, id);
	nfa_epsilon_edge(table, ini, acc);
	return NFA(ini, acc);
}

/* creat a one-char nfa */	
NFA creat_nfa_c(NFA_TABLE table, NFA_STATE_ID *id, char c)
{
	NFA_STATE_ID ini = creat_nfa_state(table, id);
	NFA_STATE_ID acc = creat_nfa_state(table, id);
	nfa_c_edge(table, ini, acc, c);
	return NFA(ini, acc);
}



/* concatenation */
NFA nfa_concat(NFA_TABLE table, NFA first, NFA second)
{
	nfa_epsilon_edge(table, first.accept, second.initial);
	return NFA(first.initial, second.accept);
}

/* union */
NFA nfa_union(NFA_TABLE table, NFA_STATE_ID *id, NFA first, NFA second)
{
	NFA_STATE_ID ini = creat_nfa_state(table, id);
	NFA_STATE_ID acc = creat_nfa_state(table, id);

	nfa_epsilon_edge(table, ini, first.initial);
	nfa_epsilon_edge(table, ini, second.initial);
	nfa_epsilon_edge(table, first.accept, acc);
	nfa_epsilon_edge(table, second.accept, acc);

	return NFA(ini, acc);
}

/* kleen star */
NFA nfa_star(NFA_TABLE table, NFA_STATE_ID *id, NFA n)
{
	NFA_STATE_ID ini = creat_nfa_state(table, id);
	NFA_STATE_ID acc = creat_nfa_state(table, id);

	nfa_epsilon_edge(table, ini, acc);
	nfa_epsilon_edge(table, ini, n.initial);
	nfa_epsilon_edge(table, n.accept, n.initial);
	nfa_epsilon_edge(table, n.accept, acc);

	return NFA(ini, acc);
}
/* postive clocure */
NFA nfa_pos_clo(NFA_TABLE table, NFA_STATE_ID *id, NFA n)
{
	NFA_STATE_ID ini = creat_nfa_state(table, id);
	NFA_STATE_ID acc = creat_nfa_state(table, id);

	nfa_epsilon_edge(table, ini, n.initial);
	nfa_epsilon_edge(table, n.accept, n.initial);
	nfa_epsilon_edge(table, n.accept, acc);

	return NFA(ini, acc);
}

/* zero or one */
NFA nfa_zero_one(NFA_TABLE table, NFA_STATE_ID *id, NFA n)
{
	NFA_STATE_ID ini = creat_nfa_state(table, id);
	NFA_STATE_ID acc = creat_nfa_state(table, id);

	nfa_epsilon_edge(table, ini, acc);
	nfa_epsilon_edge(table, ini, n.initial);
	nfa_epsilon_edge(table, n.accept, acc);

	return NFA(ini, acc);
}

/*



*/
NFA regex_to_nfa(queue<RE> &regex, NFA_TABLE table, NFA_STATE_ID *id)
{
	stack<re_type> opstk;
	stack<NFA> nfa;
	RE re;
	NFA temp;

	while (!regex.empty()) {
		re = regex.front();
		regex.pop();

		switch (re.type) {
		case character:
			temp = creat_nfa_c(table, id, re.c);
			nfa.push(temp);
			break;
		case right_pare:
			while (1) {
				re_type op = opstk.top();
				opstk.pop();
				if (op == left_pare)
					break;
				else
					foo(table, id, op, nfa);
			}
			break;
		case left_pare:
			opstk.push(left_pare);
			break;
		default:
			while (!opstk.empty() &&
					re_prio(re.type) < re_prio(opstk.top()) ) {
				re_type op = opstk.top();
				opstk.pop();
				foo(table, id, op, nfa);
			}
			opstk.push(re.type);
		}
	}

	while (!opstk.empty()) {
		re_type op = opstk.top();
		opstk.pop();
		foo(table, id, op, nfa);
	}

	return nfa.top();
}


void foo(NFA_TABLE table, NFA_STATE_ID *id, 
         re_type op, stack<NFA> &nfa)
{
	NFA nfa1;
	NFA nfa2;
	NFA nfa3;

	switch (op) {
	case concat :
		nfa1 = nfa.top();
		nfa.pop();
		nfa2 = nfa.top();
		nfa.pop();
		nfa3 = nfa_concat(table, nfa2, nfa1);
		nfa.push( nfa3);
		break;
	case uni :
		nfa1 = nfa.top();
		nfa.pop();
		nfa2 = nfa.top();
		nfa.pop();
		nfa3 = nfa_union(table, id, nfa2, nfa1);
		nfa.push( nfa3);
		break;
	case star :
		nfa1 = nfa.top();
		nfa.pop();
		nfa3 = nfa_star(table, id, nfa1);
		nfa.push( nfa3 );
		break;
	case pos_clo :
		nfa1 = nfa.top();
		nfa.pop();
		nfa.push( nfa_pos_clo(table, id, nfa1));
		break;
	case zero_one :
		nfa1 = nfa.top();
		nfa.pop();
		nfa.push( nfa_zero_one(table, id, nfa1));
		break;
	} /* end of swith op */
}




/*
  closure

*/

set<char> nfa_char_set(NFA_TABLE table, NFA nfa)
{
	set<char> char_set;
	idmap state;
	idmap::iterator i;
	NFA_EDGE *e;
	bool all_taged;

	state.insert(idpair(nfa.initial, false));
	while(1) {
		all_taged = true;
		for (i=state.begin(); i!=state.end(); i++)
			if (!i->second) {
				all_taged = false;
				break;
			}
		if (all_taged) 
			return char_set;

		i->second = true;		/* tag */
		for (e = table[i->first].first_edge; e != NULL; e = e->next) {
			state.insert(idpair(e->adjvex, false));
			if (!e->cost.epsilon)
				char_set.insert(e->cost.c);
		}
	}
}


/* 
  nfa_adj_e
  get all nfa states can be arrived via epsilon edge from give states
*/
set<NFA_STATE_ID> nfa_adj_e(NFA_TABLE table, NFA_STATE_ID id)
{
	set<NFA_STATE_ID> s;
	NFA_EDGE *e;
	
	for (e = table[id].first_edge; e != NULL; e = e->next) {
		if (e->cost.epsilon)
			s.insert(e->adjvex);
	}
	return s;
}


/*
  nfa_e_closure
  the epsilon closure of a set of nfa states
*/
set<NFA_STATE_ID> nfa_e_closure(NFA_TABLE table, set<NFA_STATE_ID> sns)
{
	set<NFA_STATE_ID> taged_states;
	set<NFA_STATE_ID>::iterator i;
	NFA_STATE_ID state;
	NFA_EDGE *e;

	while(!sns.empty()) {
		i = sns.begin();
		state = *i;
		taged_states.insert(state);
		sns.erase(i);

		for (e = table[state].first_edge; e != NULL; e = e->next) {
			if (e->cost.epsilon ) {
				if (!taged_states.count(e->adjvex))
					sns.insert(e->adjvex);
			}
		}
	}
	return taged_states;
}

/*
  nfa_adj_c
  get the nfa states can be arrived via c edge from give states 
*/
NFA_STATE_ID nfa_adj_c(NFA_TABLE table, NFA_STATE_ID id, char c)
{
	NFA_EDGE *e;
	for (e = table[id].first_edge; e != NULL; e = e->next)
		if (! e->cost.epsilon && e->cost.c == c)
			return e->adjvex;
	return -1;	
}

/* 
  nfa_cset
  get the set arrived via character from given set.
  used when nfa to dfa.
*/
set<NFA_STATE_ID> nfa_cset(NFA_TABLE table, set<NFA_STATE_ID> sns, char c)
{
	set<NFA_STATE_ID> s;
	set<NFA_STATE_ID>::iterator i;
	NFA_STATE_ID adj;

	for (i=sns.begin(); i!=sns.end(); i++) {
		adj = nfa_adj_c(table, *i, c);
		if (adj != -1)
			s.insert(adj);
	}
	
	return s;
}








/****************************************
  FOR TEST
*****************************************/
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
