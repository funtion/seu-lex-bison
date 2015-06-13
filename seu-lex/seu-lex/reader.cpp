#include "stdafx.h"
#include "reader.h"


string readUntil(FILE *file, const string& s)
{
	string result = "";
	int c;
	while ((c=fgetc(file)) != EOF){
		result += (char)c;
		if (result.length() >= s.length()){
			const string& last = result.substr(result.length() - s.length(), s.length());
			if (last == s){
				result = result.substr(0, result.length() - s.length());
				break;
			}
		}
	}
	return result;
}



string read_block(FILE *file, bool *section_end)
{
	string block = "";
	int ch, state = 0;
	char c;

	if (*section_end)
		return block;

	while ((ch = fgetc(file)) != EOF) {
		c = (char) ch;

		switch (state) {
		case 0:
			switch (c) {
			case '%':
				state = 1;
				break;
			default:
				break;
			}
			break;
		case 1:
			switch (c) {
			case '{':
				state = 2;
				break;
			case '%':
				*section_end = true;
				return block;
				break;
			default:
				state = 0;
			}
			break;
		case 2:
			switch (c) {
			case '%':
				state = 3;	
				break;
			default:
				block += c;
			}
			break;
		case 3:
			switch (c) {
			case '}':
				return block;
				break;
			case '%':
				block += '%';
				/* still state 3 */
				break;
			default:
				block += '%';
				block += c;
				break;
			}
			break;
		}
	}

	return block;
}

/*
  read_name -- read name of a regex
  name consist of digit, letter and '_', but can not begin with digit.
*/
string read_name(FILE *file, bool *section_end)
{
	string name = "";
	int ch;
	char c;
	int state = 0;

	if (*section_end)
		return name;

	while ((ch = fgetc(file)) != EOF) {
		c = (char) ch;

		switch (state) {
		case 0:
			if (isspace(c))
				break;
			else if (isalpha(c) || c == '_') {
				name += c;
				state = 1;
			} else if (c == '%') {
				state = 2;
			} else {
				printf("error: invalid name\n");
				exit(1);
			}
			break;
		case 1:
			if (isalnum(c) || c == '_')
				name += c;
			else if (isspace(c))
				return name;
			else {
				printf("error: invalid name\n");
				exit(1);
			}
			break;
		case 2:
			if (c == '%') {
				*section_end = true;
				return name;
			} else {
				printf("error: invalid name\n");
				exit(1);
			}
		}
	}

	return name;
}

string read_line(FILE *file)
{
	int ch;
	char c;
	int state = 0;
	string s = "";

	while ((ch = fgetc(file)) != EOF) {
		c = (char) ch;
		
		switch (state) {
		case 0:
			switch (c) {
			case ' ':
			case '\t':
				/* ignore */
				break;
			case '\n':
				return s;
				break;
			default:
				state = 1;
				s += c;
			}
			break;
		case 1:
			if (isspace(c))
				return s;
			else
				s += c;
			break;
		default:
			return s;
		}
	}

	return s;

}

void read_definition(FILE *file, bool *section_end, map<string, string> &definitions)
{
	string name = "";
	string regex = "";

	definitions.clear();
	while (!*section_end) {
		name = read_name(file, section_end);
		if (*section_end)
			break;
		else
			regex = read_line(file);
		definitions[name] = regex;
	}
}



string read_regex(FILE *file, bool *section_end)
{
	
	int ch;
	char c;
	int state = 0;
	string regex = "";
	bool escape = false;
	bool quote = false;
	bool bucket = false;

	if (*section_end)
		return regex;

	while ((ch = fgetc(file)) != EOF) {
		c = (char) ch;
		switch (state) {
		case 0:
			if (c == '%')
				state = 2;
			else if (!isspace(c)) {
				state = 1;
				regex += c;
				if (c == '\\')
					escape = true;
				if (c == '"')
					quote = true;
				if (c == '[') 
					bucket = true;
			}
			break;
		case 1:
			if (c == '\n') {
				cout << "error: no action for regex: " << regex << endl;
				exit(1);
			}
			if (escape) {
				escape = false;
				regex += c;
				break;
			}
			if (c == '\\') {
				escape = true;
				regex += c;
				break;
			}

			if (quote) {
				if (c == '"')
					quote = false;
				regex += c;
				break;
			}
			if (bucket) {
				if (c == ']')
					bucket = false;
				regex += c;
				break;
			}

			if (c == '"')
				quote = true;
			if (c == '[') 
				bucket = true;
			if (c == ' ' || c == '\t')
				return regex;
			regex += c;
			break;
		case 2:
			if (c == '%') {
				state = 3;
			} else if (c == ' ' || c == '\t') {
				regex += '%';
				return regex;
			} else if (c == '\n') {
				cout << "error: no action for regex: % "<< endl;
				exit(1);
			} else {
				regex += '%';
				regex += c;
				state = 1;
			}
			break;
		case 3:
			if (c == '\n') {
				*section_end = true;
				return regex;
			} else if (c == ' ' || c == '\t') {
				regex += "%%";
				return regex;
			} else {
				regex += "%%";
				state = 1;
			}
		}
	}

	return regex;
}


string read_action(FILE *file)
{
	int ch;
	char c;
	int brace = 0;
	int state = 0;
	string action = "";

	while ((ch = fgetc(file)) != EOF) {
		c = (char) ch;

		switch (state) {
		case 0:
			switch (c) {
			case ' ':
			case '\t':
				break;
			case '\n':
				return action;
				break;
			case '{':
				brace += 1;
				action += c;
				state = 1;
				break;
			default:
				action += c;
				state = 2;
			}
			break;
		case 1:
			action += c;
			switch (c) {
			case '{':
				brace++;
				break;
			case '}':
				brace--;
				if (brace == 0) 
					return action;
				break;
			}
			break;
		case 2:
			if (c == '\n')
				return action;
			else
				action += c;
			break;
		}
	}

	return action;
}


string read_code(FILE *file)
{
	int ch;
	string code = "";

	while ((ch = fgetc(file)) != EOF)
		code += (char) ch;

	return code;
}