#include "stdafx.h"
#include "dfa.h"

DFA::DFA()
{
	id = 0;

}

DFA::~DFA()
{

}

void DFA::add_states()
{
	states.push_back(DFA_STATE());
	id++;
}

void DFA::add_edge(DFA_STATE_ID begin, char c, DFA_STATE_ID end)
{
	if (begin < states.size() && end < states.size())
		states[begin].push_front(DFA_EDGE(c, end));
}



void DFA::nfa_to_dfa(NFA_TABLE table, NFA nfa)
{
	id = 0;
	states.clear();
	accept.clear();

	vector<idmap> v;	/* dfa state <--> nfa closure */
	// vector<idmap>::vi;
	vector<idmap>::size_type vt1, vt2;
	idmap imtemp;
	
	bool all_taged;
	bool new_state;
	set<char> char_set = nfa_char_set(table, nfa);
	set<char>::iterator csi;

	imtemp.insert(idpair(nfa.initial, false));	/* s0 */
	v.push_back(nfa_e_closure(table, imtemp));	/* s0 epsilon closure */
	add_states();

	for (vt1=0; vt1<v.size(); vt1++) {
		for (csi = char_set.begin(); csi != char_set.end(); csi++) {		/* for every char */
			imtemp = nfa_e_closure(table, nfa_cset(table, v[vt1], *csi));	/* get epsilon closure */
			if (imtemp.empty())
				continue;

			new_state = true;
			for (vt2=0; vt2<v.size(); vt2++)
				if (imtemp == v[vt2]) {			/* not new state */
					add_edge(vt1, *csi, vt2);	/* add edge */
					new_state = false;
					break;
				}
			if (new_state) {					/* new state */
				v.push_back(imtemp);
				add_states();
				add_edge(vt1, *csi, states.size()-1);
				if (imtemp.find(nfa.accept) != imtemp.end())	/* is accept state? */
					accept.insert(states.size()-1);
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
	set<DFA_STATE_ID>::iterator acci;
	printf("accept state: \n");
	for (acci = accept.begin(); acci != accept.end(); acci++)
		printf("%d ", *acci);
	printf("\n");
}

/**********************************************
  FOR TEST
***********************************************/
void DFA::print_dfa()
{
	vector<idmap>::size_type s;
	DFA_STATE::iterator e;

	printf("\n=======================\n");
	printf("   DFA\n");
	printf("=======================\n");
	for (s = 0; s < states.size(); s++) {
		printf("<%d>: ", s);
		for (e = states[s].begin(); e != states[s].end(); e++) {
			printf("[%c %d] ", e->c, e->adjvex);
		}
		printf("\n");
	}
}