#include "stdafx.h"
#include "dfa.h"

void dfa_state_ini(DFA_STATE state)
{
	int i;
	for (i = 0; i < CHAR_NUM; i++)
		state[i] = -1;
}

void dfa_ini(DFA &dfa)
{
	dfa.id = 0;
	int i;
	for (i = 0; i < MAX_DFA_STATE; i++)
		dfa.tbl[i] = NULL;
}

void dfa_clear(DFA &dfa)
{
	int i;
	for (i = 0; i < MAX_DFA_STATE; i++)
		if (dfa.tbl[i] != NULL)
			free(dfa.tbl[i]);
	dfa.id = 0;
}




DFA_STATE_ID dfa_add_states(DFA &dfa)
{
	if (dfa.id < MAX_DFA_STATE) {
		DFA_STATE new_state = (DFA_STATE_ID *) malloc(sizeof(DFA_STATE_ID) * CHAR_NUM);
		dfa_state_ini(new_state);
		dfa.tbl[dfa.id] = new_state;
		dfa.id ++;
		return dfa.id - 1;
	} else
		return -1;
	
}

void dfa_add_edge(DFA &dfa, DFA_STATE_ID begin, char c, DFA_STATE_ID end)
{
	if (begin < dfa.id && end < dfa.id)
		dfa.tbl[begin][toascii(c)] = end;
}




void nfa_to_dfa(NFA_TABLE nfa_tbl, NFA nfa, DFA &dfa)
{
	dfa_clear(dfa);

	vector<idmap> v;	/* dfa state <--> nfa closure */
	vector<idmap>::size_type vt1, vt2;
	idmap imtemp;
	
	set<char> char_set = nfa_char_set(nfa_tbl, nfa);
	set<char>::iterator csi;

	DFA_STATE_ID new_state;
	bool all_taged;
	bool is_new_state;

	imtemp.insert(idpair(nfa.initial, false));	/* s0 */
	v.push_back(nfa_e_closure(nfa_tbl, imtemp));	/* s0 epsilon closure */
	dfa_add_states(dfa);

	for (vt1=0; vt1<v.size(); vt1++) {
		for (csi = char_set.begin(); csi != char_set.end(); csi++) {		/* for every char */
			imtemp = nfa_e_closure(nfa_tbl, nfa_cset(nfa_tbl, v[vt1], *csi));	/* get epsilon closure */
			if (imtemp.empty())
				continue;

			is_new_state = true;
			for (vt2=0; vt2<v.size(); vt2++)
				if (imtemp == v[vt2]) {			/* if not new state */
					dfa_add_edge(dfa, vt1, *csi, vt2);	/* add edge */
					is_new_state = false;
					break;
				}
			if (is_new_state) {					/* if new state */
				v.push_back(imtemp);
				new_state = dfa_add_states(dfa);
				dfa_add_edge(dfa, vt1, *csi, new_state);
				if (imtemp.find(nfa.accept) != imtemp.end())	/* is accept state? */
					;
				//	accept.insert(states.size()-1);
			}
		}
	}

	/* for test *********************************/
	idmap::iterator imi;
	printf("\ndfa states <-------> nfa closure\n");
	for (vt1 = 0; vt1 < v.size(); vt1++) {
		printf("%d:", vt1);
		for (imi=v[vt1].begin(); imi!=v[vt1].end(); imi++)
			printf("%d ", imi->first);
		printf("\n");
	}
	// set<DFA_STATE_ID>::iterator acci;
	// printf("accept state: \n");
	// for (acci = accept.begin(); acci != accept.end(); acci++)
	// 	printf("%d ", *acci);
	// printf("\n");
}


void dfa_mini(DFA &dfa)
{
	
}


/**********************************************
  FOR TEST
***********************************************/
void print_dfa(DFA &dfa)
{
	int i, c;

	printf("\n=======================\n");
	printf("   DFA\n");
	printf("=======================\n");
	
	for (i = 0; i < dfa.id; i++) {
		printf("<%d> :", i);
		for (c = 0; c < 128; c++)
			if (dfa.tbl[i][c] != -1)
				printf("[%c %d] ", c, dfa.tbl[i][c]);
		printf("\n");
	}
}