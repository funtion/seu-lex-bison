#include "stdafx.h"
#include "dfa.h"

#ifndef _WRITER_H
#define _WRITER_H


void generate_file_path(char *in, char *out);

void write_dfa(FILE *yyout, DFA &dfa, vector<THESEUS> &ships);
void write_accept(FILE *yyout, DFA &dfa);
void write_next(FILE *yyout, DFA &dfa);
void write_action(FILE *yyout, vector<THESEUS> &ships);


#endif