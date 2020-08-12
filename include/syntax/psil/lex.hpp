#ifndef __LEX_H_
#define __LEX_H_

#include <string>

enum tok_type {
	begin = 'B', // sentinel
	end = 'E', // sentinel
	symbol = 'a', // any symbol, represetented by a in grammar
	left_paren = '(',
	right_paren = ')',
	left_brack = '[',
	right_brack = ']',
	left_brace = '{',
	right_brace = '}',
	left_chev = '<',
	right_chev = '>',
	dot = '.',
	dollar = '$',
	colon = ':',
	hash = '#',
	at = '@'
};

struct tok_list {
	std::string sym;
	enum tok_type type;
	struct tok_list *prev;
	struct tok_list *next;
};

struct tok_list *lex_def(std::istream &fin); // returns a list of tokens from the input
void tok_print(struct tok_list *list); // prints a representation of the tokens to cout
struct tok_list *lex_next(); // returns next token
void lex_reset(); // goes back to start

#endif // __LEX_H_