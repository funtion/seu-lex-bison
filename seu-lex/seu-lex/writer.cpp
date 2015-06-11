#include "stdafx.h"
#include "writer.h"

//extern char head[];
//extern char syylex[];
char head[] = "\n\
/* standard c include */\n\
#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <tchar.h>\n\
#include <ctype.h>\n\
\n\
/* global varibles */\n\
int yyleng;\n\
FILE *yyin, *yyout;\n\
\n\
char yytext[1024] = {'\\0'};\n\
\n\
unsigned int current_state, last_accept_state, next_state;\n\
\n\
";


char syylex[] = "\n\
\n\
/* yylex */\n\
int yylex() {\n\
\n\
	int ch;\n\
	char c;\n\
	int pos;\n\
	int back;\n\
\n\
	if (yyin == NULL)\n\
		yyin = stdin;\n\
\n\
	if (yyout == NULL)\n\
		yyout= stdout;\n\
\n\
	while (1) {\n\
\n\
		current_state = 1;\n\
		last_accept_state = 0;\n\
\n\
		pos = 0;\n\
		back = 0;\n\
\n\
		/* match */\n\
		while ( (ch = fgetc(yyin)) != EOF ) {\n\
			c = (char) ch;\n\
			yytext[pos++] = c;\n\
			next_state = next[current_state][toascii(c)];\n\
			if (next_state) {\n\
				current_state = next_state;\n\
				if (accept[current_state]) {\n\
					last_accept_state = current_state;\n\
					back = 0;\n\
				} else {\n\
					back++;\n\
				}\n\
			} else \n\
				break;\n\
		}\n\
\n\
		back++;\n\
		while (back--) {\n\
			--pos;\n\
			ungetc(yytext[pos], yyin);\n\
			yytext[pos] = '\\0';\n\
		}\n\
";


void write_accept(FILE *yyout, DFA &dfa)
{

	fprintf(yyout, "unsigned int accept[] = { \n\t0, \n\t");

	unsigned int i;
	for (i = 1; i < dfa.id; i++) {
		if (dfa.states[i].is_accept)
			fprintf(yyout, "1,\t");
		else
			fprintf(yyout, "0,\t");

		if (i % 16 == 0)
			fprintf(yyout, "\n");
	}

	fprintf(yyout, "\n};\n");
}

void write_next(FILE *yyout, DFA &dfa)
{
	unsigned int i;
	int c;

	fprintf(yyout, "unsigned int next[][128] = { {0}, \n");

	for (i = 1; i < dfa.id; i++) {
		fprintf(yyout, "{\n");
		for (c = 0; c < 128; c++) {
			fprintf(yyout, "%d,\t", dfa.states[i].next[c]);
			if (c%16 == 15)
				fprintf(yyout, "\n");
		}
		fprintf(yyout, "},\n");
	}

	fprintf(yyout, "};\n");
}

void write_action(FILE *yyout, vector<THESEUS> &ships)
{
	fprintf(yyout, "/* action */\nswitch (last_accept_state) {\n");
	vector<THESEUS>::iterator shipsit;
	list<DFA_STATE_ID>::iterator dfast;

	for (shipsit = ships.begin(); shipsit != ships.end(); shipsit++) {
		for (dfast = shipsit->dfa_states.begin(); dfast != shipsit->dfa_states.end(); dfast++) {
			fprintf(yyout, "case %d:\n", *dfast);
		}
		fprintf(yyout, "\t%s\n", shipsit->action.c_str());
		fprintf(yyout, "break;\n");
	}
	fprintf(yyout, "default:\nreturn;\n}");
}


void write_dfa(FILE *yyout, DFA &dfa, vector<THESEUS> &ships)
{
	write_accept(yyout, dfa);
	write_next(yyout, dfa);

	fprintf(yyout, "%s\n", syylex);
	write_action(yyout, ships);
	fprintf(yyout, "}\n}\n");
}



