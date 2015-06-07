#include "stdafx.h"
#include "regex.h"
#include "nfa.h"
#include "dfa.h"
#include "reader.h"

void test_nfa(void)
{
	NFA_TABLE nfat;
	ini_nfa_table(nfat);
	NFA_STATE_ID id = 0;

	NFA nfa1 = creat_nfa(nfat, &id);
	NFA nfa2 = creat_nfa_c(nfat, &id, 'a');
	NFA nfa3 = nfa_union(nfat, &id, nfa1, nfa2);
	NFA nfa4 = creat_nfa_c(nfat, &id, 't');
	NFA nfa5 = nfa_star(nfat, &id, nfa3);

	print_nfa_table(nfat, id);

	return;
}


void test_regex_check()
{
	char *regex = "ab[a-z])(t\"fsf\"*\0";
	if (regex_check(regex))
		printf("ok\n");
	else
		printf("error\n");
}

void test_preprocess()
{
	queue<RE> re_queue;
	// char regex[] = "ab(cd)*[0-3a-d]e+\0";
	char regex[] = "ab|[c-e]f*\0";
	printf("%s\n", regex);

	regex_preprocess(regex, re_queue);
	
	cout << "print the queue------------------" << endl;
	while (!re_queue.empty()) {
		print_re(&re_queue.front());
		re_queue.pop();
	}

	return;
}

void test_regex2nfa()
{
	char regex[] = "ab|[c-e]f*\0";
	queue<RE> re_queue;
	NFA_TABLE table;
	NFA_STATE_ID id;
	/* initial */
	ini_nfa_table(table);
	id = 0;

	printf("check...\n");
	if (regex_check(regex))
		printf("ok\n");
	else {
		printf("error\n");
		return;
	}

	printf("pre-proccessing...\n");
	regex_preprocess(regex, re_queue);

	printf("\nregex to nfa...\n");
	NFA nfa = regex_to_nfa(re_queue, table, &id);
	printf("\nprint nfa table...\n");
	print_nfa_table(table, id);

	/* test closure *********************/
	idmap m0, m1;
	m0.insert(idpair(nfa.initial, false));

	m1 = nfa_e_closure(table, m0);
	idmap::iterator i;
	printf("e-closure of s0: ");
	for (i=m1.begin(); i!=m1.end(); i++)
		printf("%d ", i->first);

	/* test char set ********************/
	set<char> cs = nfa_char_set(table, nfa);
	set<char>::iterator csi;
	printf("\nchsr set of nfa:");
	for (csi = cs.begin(); csi != cs.end(); csi++)
		printf("%c ", *csi);
	printf("\n");

	/* test dfa **************************/
	printf("\ncreate dfa...\n");
	DFA dfa;
	dfa_ini(dfa);
	printf("\nnfa to dfa.....\n");
	nfa_to_dfa(table, nfa, dfa);
	print_dfa(dfa);
}


void test_char()
{
	int c;
	// for (c='!'; c<'~'; c++)
	// 	printf("%c :%d\n", c, c);
	for (c=1; c<128; c++)
		printf("%c :%d\n", c,c);
}


void test_reader()
{
	FILE* lexl = fopen("E:\\lex.l", "r");

	string user_code;
	string regex;
	string action;
	string code;

	map<string, string> definitions;
	map<string, string>::iterator iter;

	map<string, string> regex_action;
	map<string, string>::iterator ra;

	bool def_sec_end = false;
	bool rule_sec_end = false;

	user_code = read_block(lexl, &def_sec_end);
	if (!def_sec_end)
		read_definition(lexl, &def_sec_end, definitions);

	while (!rule_sec_end) {
		regex = read_regex(lexl, &rule_sec_end);
		action = read_action(lexl);
		if (regex != "")
			regex_action[regex] = action;
	}

	code = read_code(lexl);

	cout << "literal block ----------------------------------" << endl;
	cout << user_code << endl;

	cout << "definitions ------------------------------------" << endl;
	for (iter = definitions.begin(); iter != definitions.end(); iter++) {
		cout << iter->first << " <---> " ;
		cout << iter->second << endl;
	}

	cout << "regex  action ----------------------------------" << endl;
	for (iter = regex_action.begin(); iter != regex_action.end(); iter++) {
		cout << iter->first << " <---> " ;
		cout << iter->second << endl;
	}

	cout << "code section ----------------------------------" << endl;
	cout << code << endl;

	fclose(lexl);
}