// seu-lex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "test.h"




int main(int argc, char* argv[])
{
	// test_nfa();
	//test_regex_check();
	//test_preprocess();
	//test_regex2nfa();
	//test_char();
	//test_reader();
	//test_theseus();
	//test_substitute();
	//seu_lex(argc, argv);

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
	char out_path[128];

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
		printf(" <----------> ");
		for (dfast = shipsit->dfa_states.begin(); dfast != shipsit->dfa_states.end(); dfast++) 
			printf("%d ", *dfast);
		printf("\n");
	}

	/* dfa mini ****************************************/


	/* write to lex.yy.c ***********************************************/

	generate_file_path(argv[1], out_path);
	yyout = fopen(out_path, "w");
	if (yyout == NULL) {
		fprintf(stderr, "open file f:\\lex.yy.l failed \n");
		exit(1);
	}

	fprintf(yyout, "%s\n", head);
	fprintf(yyout, "%s\n", literal_block.c_str());
	write_dfa(yyout, dfa, ships);
	fprintf(yyout, "%s\n", subroutines.c_str());

	return 0;
}