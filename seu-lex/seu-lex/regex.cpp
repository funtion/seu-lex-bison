#include "stdafx.h"
#include "regex.h"

int re_prio(re_type op)
{
	switch (op) {
	case star :
	case pos_clo :
	case zero_one :
		return 3;
	case concat:
		return 2;
	case uni:
		return 1;
	case left_pare:
		return 0;
	default:
		return -1;
	}
}



/* regex_check - check the syntax of a regular expression */
bool regex_check(string &regex)
{
	char c;
	int parenthesis = 0;
	int bracket = 0;
	int brace = 0;
	bool quote = false;
	bool escape = false;
	int length, i;

	length = regex.length();
	for (i = 0; i < length; i++) {
		c = regex[i];

		if (escape) {
			escape = false;
			continue;
		}

		if (quote) {
			if (c == '"')
				quote = false;
			continue;
		}

		switch (c) {
		case '\\':
			escape = true;
			break;
		case '"':
			quote = true;
			break;
		case '(':
			parenthesis++;
			break;
		case ')':
			if (parenthesis > 0)
				parenthesis--;
			else
				return false;
			break;
		case '[':
			bracket++;
			break;
		case ']':
			if (bracket)
				bracket--;
			else
				return false;
			break;
		case '{':
			brace++;
			break;
		case '}':
			if (brace)
				brace--;
			else
				return false;
			break;
		}
	}

	if (quote
		|| parenthesis != 0 
		|| bracket != 0 
		|| brace != 0)
		return false;
	else
		return true;

}





void regex_substitute(string &regex, map<string, string> &definitions)
{
	bool sub = false;
	bool escape = false;
	bool bucket = false;	
	bool literal = false;
	int pos, len;
	int i;
	string name = "";

	for (i = 0; i < regex.length(); i++) {

		if (sub) {
			if (regex[i] == '}') {
				len = i - pos + 1;
				name = regex.substr(pos+1, len-2);
				if (definitions.find(name) != definitions.end()) {
					regex.replace(pos, len, definitions[name]);
					i += (definitions[name].length() - len );
				} else {
					fprintf(stderr, "regex substitute no found: %s\n", "");
					exit(1);
				}
				sub = false;
			}
			continue;
		}

		if (escape) {
			escape = false;
			continue;
		}
		if (regex[i] == '\\') {
			escape = true;
			continue;
		}

		if (bucket) {
			if (regex[i] == ']')
				bucket = false;
			continue;
		}
		if (literal) {
			if (regex[i] == '"')
				literal = false;
			continue;
		}

		switch (regex[i]) {
		case '[':
			bucket = true;
			break;
		case '"':
			literal = true;
			break;
		case '{':
			pos = i;
			sub = true;
		}
	}
}


/*
  regex_preprocess
  transfer a expanded form regular expression to a queue of RE struct.
  remove all brackets, braces, quotes and so on.
*/
/*
  TODO:
  add concat when between quotes
*/
void regex_preprocess(string &regex, queue<RE> &re_queue)
{
	char c;
	bool escape = false;	/* after \     */ 
	bool scope = false;		/* between [ ] */
	bool literal = false;	/* between " " */
	bool add_concat = false;/* need to add a concat before procces next */
	bool add_uni = false;
	RE temp;
	RE conc;
	int length, i;

	length = regex.length();
	for (i = 0; i < length; i++) {
		c = regex[i];

		/* */
		if (escape) {
			switch (c) {
			case 'n':
				re_queue.push( RE('\n') );
				break;
			case 't':
				re_queue.push( RE('\t') );
				break;
			default:
				re_queue.push( RE(c) );
			}
			escape = false;
			continue;
		}
		if (c == '\\') {
			if (add_concat)
				re_queue.push( RE(concat) );
			else if (scope && add_uni)
				re_queue.push( RE(uni) );
			else if (scope && !add_uni)
				add_uni = true;
			escape = true;
			continue;
		}

		if (literal) {
			if (c == '"') {
				literal = false;	/* end of literal */
				re_queue.push( RE(right_pare) );
			} else 					/* literal */
				re_queue.push( RE(c) );
			
			continue;
		}

		if (scope) {
			if (c == ']') {				/* end of scope */
				scope = false;		
				re_queue.push( RE(right_pare) );
				add_concat = true;
				add_uni = false;
				continue;
			} else if (c == '-') {
				char lc = regex[i-1];
				char hc = regex[i+1];
				i++;

				if (isalnum(lc) && isalnum(hc) && lc<hc) {
					for (++lc; lc <= hc; lc++) {
						re_queue.push( RE(uni) );		/* push union */
						re_queue.push( RE(lc) );
					}
				} else {
					fprintf(stderr, "regex syntex error: %s\n", regex.c_str());
					exit(1);
				}

				continue;
			} else {
				if (add_uni)
					re_queue.push( RE(uni) );
				else
					add_uni = true;

				re_queue.push( RE(c) );				/* push char */
				continue;
			}			
		}

		/* */
		switch (c) {
		case '"':
			literal = true;
			re_queue.push( RE(left_pare) );
			continue;
			break;
		case '[':
			scope = true;
			if (add_concat)	
				re_queue.push( RE(concat) );
			re_queue.push( RE(left_pare) );
			continue;
			break;
		}


		/* operator */
		switch (c) {
		case '*':					/* kleen closure */
			re_queue.push( RE(star) );
			continue;
			break;
		case '|':					/* union */
			re_queue.push( RE(uni) );
			add_concat = false;
			continue;
			break;
		case '(':					/* left pare */
			if (add_concat) 				/* need to insert a concat */
				re_queue.push( RE(concat) );
			re_queue.push( RE(left_pare) );
			add_concat = false;
			continue;
			break;		
		case ')':					/* right pare */
			re_queue.push( RE(right_pare) );
			continue;
			break;
		case '+':					/* postive clusure + */
			re_queue.push( RE(pos_clo) );
			continue;
			break;
		}


		/* symbol */
		if (add_concat)					/* need to insert a concat */
			re_queue.push( RE(concat) );
		re_queue.push( RE(c) );
		add_concat = true;
	
	}
}


/* for test */
void print_re_type(re_type rt)
{
	switch (rt) {
	case concat:
		printf("concat\n");
		break;
	case star:
		printf("star\n");
		break;
	case uni:
		printf("union\n");
		break;
	case left_pare:
		printf("left pare\n");
		break;
	case right_pare:
		printf("right pare\n");
		break;
	case pos_clo:
		printf("positive closure\n");
		break;
	default:
		printf("\n");
	}
}

void print_re(RE *r)
{
	if (r->type == character) {
		printf("character: %c\n", r->c);
	}
	else {
		printf("operator : ");
		print_re_type(r->type);
	}
}
