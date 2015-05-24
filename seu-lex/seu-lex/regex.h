#ifndef _REGEX_H
#define _REGEX_H


enum re_type {
	//todo: order by priority
	character,

	concat,		/* . */
	uni,		/* | */
	star,		/* * */
	pos_clo,	/* + */
	zero_one,	/* ? */
	left_pare,	/* ( */
	right_pare,	/* ) */
};

int re_prio(re_type op);


/* 
  regular expression element
  character or operator
*/
struct RE
{
	enum re_type type;	/* operator or operant */
	char c;				/* character */


	RE(){}

	RE(char ch)
	{
		type = character;
		c = ch;
	}

	RE(enum re_type t)
	{
		type = t;
	}

	RE(const RE& r)
	{
		type = r.type;
		c = r.c;
	}
};



/* check the syntax of a regular expression */
bool regex_check(char *regex);

/* pre-proccessing */
void regex_preprocess(char* regex, queue<RE> &re_queue);



/* for test */
void print_re(RE *r);
void print_re_type(re_type rt);
// void print_opstk(stack<re_type> opstk);
#endif