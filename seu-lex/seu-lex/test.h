#include "stdafx.h"
#include "regex.h"
#include "nfa.h"
#include "dfa.h"
#include "reader.h"
#include "writer.h"


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




void test_preprocess()
{
	queue<RE> re_queue;
	// char regex[] = "ab(cd)*[0-3a-d]e+\0";
	string regex = "ab|[c-e]f*";
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
	string regex = "[ \\t\\n]";
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
	set<NFA_STATE_ID> m0, m1;
	m0.insert(nfa.initial);

	m1 = nfa_e_closure(table, m0);
	set<NFA_STATE_ID>::iterator i;
	printf("e-closure of s0: ");
	for (i=m1.begin(); i!=m1.end(); i++)
		printf("%d ", *i);

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
//	nfa_to_dfa(table, nfa, dfa, ships);
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


void test_theseus()
{
	string regex = "abc";
	string action = "return 0;";

	THESEUS ship(regex, action);
	cout << ship.regex << endl;
	cout << ship.action << endl;
}


void test_substitute()
{
	map<string, string> defs;

	defs.insert(pair<string, string>("name", "regex"));
	string regex = "lfda{name}fdsl";

	regex_substitute(regex, defs);
	cout << regex << endl;
}


void seu_lex(int argc, char* argv[])
{
	/********     Global Variables     **********************************/
	/* definition section */
	string literal_block;
	map<string, string> definitions;
	/* rules section */
	vector<THESEUS> ships;
	vector<THESEUS>::iterator shipsit;
	/* subroutines section */
	string subroutines;

	/* file */
	FILE* yyin;
	FILE* yyout;

	/* NFA */
	NFA_TABLE 		nfa_tbl;
	NFA_STATE_ID 	nfa_id = 0;
	queue<RE> 		req;
	NFA nfa;

	/* DFA */
	DFA dfa;

	extern char head[];
	/*********************************************************************/




	/* arguments *********************************************************/
	if (argc != 2) {
		printf("usage: %s path_of_lex.l\n", argv[0]);
		exit(1);
	}

	yyin = fopen(argv[1], "r");
	if (yyin == NULL) {
		fprintf(stderr, "open file error: %s\n", argv[1]);
		exit(1);
	}

	/* read ************************************************************/
	bool def_sec_end = false;
	bool rule_sec_end = false;
	string regex;
	string action;

	literal_block = read_block(yyin, &def_sec_end);
	if (!def_sec_end)
		read_definition(yyin, &def_sec_end, definitions);
	while (!rule_sec_end) {
		regex = read_regex(yyin, &rule_sec_end);
		action = read_action(yyin);
		if (regex != "")
			ships.push_back(THESEUS(regex, action));
	}
	subroutines = read_code(yyin);


	/* process regex substitute ************************/
	printf("substitute **************************************************\n");
	for (shipsit = ships.begin(); shipsit != ships.end(); shipsit++) {
		regex_substitute(shipsit->regex, definitions);
	}


	/* initial **********************************************************/
	ini_nfa_table(nfa_tbl);
	nfa_id = 0;
	dfa_ini(dfa);


	/* regex preprocessing; regex to nfa   *******************************/
	printf("regex to nfa ************************************************\n");
	for (shipsit = ships.begin(); shipsit != ships.end(); shipsit++) {
		if (!regex_check(shipsit->regex)) {
			fprintf(stderr, "regex syntex error: %s\n", shipsit->regex.c_str());
			exit(1);
		}
		regex_preprocess(shipsit->regex, req);
		nfa = regex_to_nfa(req, nfa_tbl, &nfa_id);
		shipsit->nfa = nfa;
	}

	/* nfa merger **************************************/
	printf("nfa merge *****************************************************\n");
	nfa = nfa_merge(nfa_tbl, &nfa_id, ships);
	printf("nfa: [%d %d] ******************************************\n", nfa.initial, nfa.accept);

	printf("\nnfa: \n");
	print_nfa_table(nfa_tbl, nfa_id);
	printf("\n\n");


	/* nfa to dfa **************************************/
	printf("nfa to dfa *****************************************************\n");
	nfa_to_dfa(nfa_tbl, nfa, dfa, ships);
	print_dfa(dfa);   ////////////////////////

	for (shipsit = ships.begin(); shipsit != ships.end(); shipsit++) { ///////////////////
		list<DFA_STATE_ID>::iterator dfast;
		printf("regex: %s", shipsit->regex.c_str());
		printf(" <----------> \n");
		for (dfast = shipsit->dfa_states.begin(); dfast != shipsit->dfa_states.end(); dfast++) 
			printf("%d ", *dfast);
		printf("\n");
	}

	/* dfa mini ****************************************/


	/* write to lex.yy.c ***********************************************/
	yyout = fopen("f:\\lex.yy.c", "w");
	if (yyout == NULL) {
		fprintf(stderr, "open file f:\\lex.yy.l failed \n");
		exit(1);
	}

	fprintf(yyout, "%s\n", head);
	fprintf(yyout, "%s\n", literal_block.c_str());
	write_dfa(yyout, dfa, ships);
	fprintf(yyout, "%s\n", subroutines.c_str());

}


