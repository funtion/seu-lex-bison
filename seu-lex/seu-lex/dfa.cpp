#include "stdafx.h"
#include "dfa.h"

/*
  nfa_merge
  merge all nfas to one
*/
NFA nfa_merge(NFA_TABLE table, NFA_STATE_ID *id, vector<THESEUS> &ships)
{
	NFA_STATE_ID ini = creat_nfa_state(table, id);
	NFA_STATE_ID acc = creat_nfa_state(table, id);

	unsigned int i;
	for (i=0; i<ships.size(); i++) {
		nfa_epsilon_edge(table, ini, ships[i].nfa.initial);
		nfa_epsilon_edge(table, ships[i].nfa.accept, acc);
	}
	
	return NFA(ini, acc);
}






void dfa_state_ini(DFA_STATE &state)
{
	int i;
	for (i = 0; i < CHAR_NUM; i++)
		state.next[i] = 0;
	state.is_accept = false;
}

void dfa_ini(DFA &dfa)
{
	int i;
	for (i = 0; i < MAX_DFA_STATE; i++)
		dfa.states[i].next = NULL;
	dfa.id = 1;
}

void dfa_clear(DFA &dfa)
{
	int i;
	for (i = 0; i < MAX_DFA_STATE; i++)
		if (dfa.states[i].next != NULL)
			free(dfa.states[i].next);
	dfa.id = 1;
}




DFA_STATE_ID dfa_add_states(DFA &dfa)
{
	if (dfa.id < MAX_DFA_STATE) {
		DFA_STATE new_state;
		new_state.next = (DFA_STATE_ID *) malloc(sizeof(DFA_STATE_ID) * CHAR_NUM);
		dfa_state_ini(new_state);
		dfa.states[dfa.id] = new_state;
		dfa.id ++;
		return dfa.id - 1;
	} else
		return 0;
	
}

void dfa_add_edge(DFA &dfa, DFA_STATE_ID begin, char c, DFA_STATE_ID end)
{
	if (begin < dfa.id && end < dfa.id)
		dfa.states[begin].next[toascii(c)] = end;
}

void dfa_set_accept(DFA &dfa, DFA_STATE_ID s)
{
	if (s < dfa.id)
		dfa.states[s].is_accept = true;
}




void nfa_to_dfa(NFA_TABLE nfa_tbl, NFA nfa, DFA &dfa, vector<THESEUS> &ships)
{
	dfa_clear(dfa);

	vector< set<NFA_STATE_ID> > v;	/* dfa state <--> nfa closure */
	vector< set<NFA_STATE_ID> >::size_type vt1, vt2;
	set<NFA_STATE_ID> sns;			/* set of nfa states */
	
	vector<THESEUS>::iterator shipsit;

	set<char> char_set = nfa_char_set(nfa_tbl, nfa);
	set<char>::iterator csi;

	DFA_STATE_ID new_state;
	bool is_new_state;

	v.push_back(sns);	/* star from 1 */
	
	sns.insert(nfa.initial);					/* s0 */
	v.push_back(nfa_e_closure(nfa_tbl, sns));	/* s0 epsilon closure */
	dfa_add_states(dfa);

	for (vt1=1; vt1<v.size(); vt1++) {
		for (csi = char_set.begin(); csi != char_set.end(); csi++) {		/* for every char */
			sns = nfa_e_closure(nfa_tbl, nfa_cset(nfa_tbl, v[vt1], *csi));	/* get epsilon closure */
			if (sns.empty())
				continue;

			is_new_state = true;
			for (vt2=1; vt2<v.size(); vt2++)
				if (sns == v[vt2]) {			/* if not new state */
					dfa_add_edge(dfa, vt1, *csi, vt2);	/* add edge */
					is_new_state = false;
					break;
				}
			if (is_new_state) {					/* if new state */
				v.push_back(sns);
				new_state = dfa_add_states(dfa);
				dfa_add_edge(dfa, vt1, *csi, new_state);
				if (sns.count(nfa.accept)) { /* is accept state? */
					dfa_set_accept(dfa, new_state);
					for (shipsit = ships.begin(); shipsit != ships.end(); shipsit++) {
						if (sns.count(shipsit->nfa.accept)) {
							shipsit->dfa_states.push_back(new_state);
							break;
						}
					}
				}
			}
		}
	}

	/* for test *********************************/
	set<NFA_STATE_ID>::iterator si;
	printf("\ndfa states <-------> nfa closure\n");
	for (vt1 = 0; vt1 < v.size(); vt1++) {
		printf("%d: ", vt1);
		for (si = v[vt1].begin(); si != v[vt1].end(); si++)
			printf("%d ", *si);
		printf("\n");
	}

}


void dfa_mini(DFA &dfa, set<DFA_STATE_ID> accept)
{
	DFA_STATE_ID state_num, s, next1,next2;
	int i, j, turn;
	int old_radix, new_radix, *old_djset, *new_djset;

	vector< list<DFA_STATE_ID> > bucket;
	list<DFA_STATE_ID> templist;
	vector< list<DFA_STATE_ID> >::iterator bi;
	list<DFA_STATE_ID>::iterator li;

	bool is_equal;		/* is 2 states equal? */

	state_num = dfa.id - 1;
	int *djset1 = new int[state_num];
	int *djset2 = new int[state_num];
	DFA_STATE_ID *collect = new DFA_STATE_ID[state_num];
	

	/*
	  in the begining, split into 2 sets based on whether is accept states.
	  set 0 for invalid transition.
	*/
	turn = 0;
	for (s = 1; s <= state_num; s++)
		djset1[s-1] = dfa.states[s].is_accept? 1:2;
	old_radix = 3;

	/* */
	while(1)
	{
		if (turn == 0) {
			old_djset = djset1;
			new_djset = djset2;
			turn = 1;
		} else {
			old_djset = djset2;
			new_djset = djset1;
			turn = 0;
		}


		/* creat buckets  */
		bucket.clear();
		for (i = 0; i < old_radix; i++)
			bucket.push_back(templist);

		/* first time distribute */
		for (s = 1; s <= state_num; s++)
			bucket[ old_djset[s-1] ].push_back( s );
		/* first time collect */
		j = 0;
		for (bi = bucket.begin(); bi != bucket.end(); bi++)
			for (li = (*bi).begin(); li != (*bi).end(); li++)
				collect[j++] = *li;

		for (i = 0; i < 128; i++) {		/* for each char */
			/* clear bucket */
			for (bi = bucket.begin(); bi != bucket.end(); bi++)
				(*bi).clear();
			/* distribute */
			for (j = 0; j < state_num; j++) {
				next1 = dfa.states[ collect[j] ].next[i];
				bucket[ old_djset[next1] ].push_back(collect[j]);
			}
			/* collect */
			j = 0;
			for (bi = bucket.begin(); bi != bucket.end(); bi++)
				for (li = (*bi).begin(); li != (*bi).end(); li++)
					collect[j++] = *li;
		}

		/* judge */
		new_radix = 1;
		new_djset[ collect[0] ] = ++new_radix;
		for (i = 1; i < state_num; i++) {
			is_equal = true;
			if (old_djset[collect[i]] != old_djset[collect[i-1]])
				is_equal = false;
			else {
				for (j = 0; j < 128; j++) {
					next1 = dfa.states[ collect[i] ].next[j];
					next2 = dfa.states[ collect[i-1] ].next[j];
					if (old_djset[next1] != old_djset[next2]) {
						is_equal = false;
						break;
					}
				}
			}

			if (is_equal)
				new_djset[ collect[j] ] = new_djset[ collect[j-1] ];
			else
				new_djset[ collect[j] ] = ++new_radix;
		}

		if (new_radix == old_radix)		/* complete mini */
			break;
		else							/* not complete */
			old_radix = new_radix;
	}

	/*
	  TODO : merge
	*/
	if (new_radix == dfa.id)
		return;

	

}


/**********************************************
  FOR TEST
***********************************************/
void print_dfa(DFA &dfa)
{
	unsigned int i;
	int c;

	printf("\n=======================\n");
	printf("   DFA\n");
	printf("=======================\n");
	
	for (i = 1; i < dfa.id; i++) {
		printf("<%d> :", i);
		for (c = 0; c < 128; c++)
			if (dfa.states[i].next[c] != 0)
				printf("[%c %d] ", c, dfa.states[i].next[c]);
		printf("\n");
	}
}