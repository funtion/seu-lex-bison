#include "stdafx.h"

#ifndef _READER_H
#define _READER_H

string read_until(FILE *file, const string& s);

/* definition section */
string read_block(FILE *file, bool *section_end);
string read_name(FILE *file, bool *section_end);
string read_line(FILE *file);
void read_definition(FILE *file, bool *section_end, map<string, string> &definitions);

/* rules section */
string read_regex(FILE *file, bool *section_end);
string read_action(FILE *file);

/* code section */
string read_code(FILE *file);

#endif