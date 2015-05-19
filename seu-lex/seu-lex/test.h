#include "stdafx.h"
#include "nfa.h"

void test_nfa(void)
{
	NFA_TABLE nfat;
	ini_nfa_table(nfat);

	NFA_STATE_ID id = 0;

	NFA nfa1 = creat_nfa(nfat, id);
	id += 2;

	NFA nfa2 = creat_nfa_c(nfat, id, 'a');
	id += 2;

	nfa_union(nfat, id, nfa1, nfa2);

	print_nfa_table(nfat, 10);


}