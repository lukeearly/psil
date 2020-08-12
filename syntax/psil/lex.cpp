#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "syntax/psil/lex.hpp"

static tok_list *first;
static tok_list *last;
static tok_list *next;

void lex_error(std::string desc)
{
	std::cerr << desc;
	exit(1);
}

char lex_esc(std::istream &fin)
{
	char n;
	if (!fin.eof()) {
		fin.get(n);
		return n;
	}
	lex_error("Unexpected EOF");
	// unreachable
}

struct tok_list *addtok(struct tok_list *list, enum tok_type type, std::string str)
{
	list->next = new struct tok_list();
	list->next->type = type;
	list->next->sym = str;
	list->next->prev = list;
	return list->next;
}

void lex_com(std::istream &fin)
{
	char n;
	while (fin.get(n)) {
		switch (n) {
		case '\n':
		case '\r':
			return;
		}
	}
	return;
}

std::string lex_str(std::istream &fin)
{
	std::string buf;
	char n;
	while (fin.get(n)) {
		switch (n) {
		case '"':
			return buf;
		case '\\':
			n = lex_esc(fin);
		default:
			buf += n;
		}
	}
	lex_error("Mismatched quotes\n");
	// unreachable
}

struct tok_list *lex_def(std::istream &fin)
{
	std::string buf;
	char n;
	first = new struct tok_list();
	first->type = begin;
	last = first;
	next = first;
	while (fin.get(n)) {
		switch (n) {
		case left_paren:
		case right_paren:
		case left_brack:
		case right_brack:
		case left_brace:
		case right_brace:
		case left_chev:
		case right_chev:
		case dot:
		case dollar:
		case colon:
		case hash:
		case at:
			if (!buf.empty()) {
				last = addtok(last, symbol, buf);
				buf = "";
			}
			last = addtok(last, (enum tok_type) n, "");
			break;
		case ' ':
		case '\n':
		case '\r':
		case '\t':
			if (!buf.empty()) {
				last = addtok(last, symbol, buf);
				buf = "";
			}
			break;
		case ';':
			if (!buf.empty()) {
				last = addtok(last, symbol, buf);
				buf = "";
			}
			lex_com(fin);
			break;
		case '"':
			if (!buf.empty()) {
				last = addtok(last, symbol, buf);
			}
			buf = lex_str(fin);
			if (!buf.empty()) {
				last = addtok(last, symbol, buf);
				buf = "";
			}
			break;
		case '\\':
			n = lex_esc(fin);
		default:
			buf += n;
		}
	}
	if (!buf.empty()) {
		last = addtok(last, symbol, buf);
	}
	last = addtok(last, end, "");
	return first;
}

//void tok_print(struct tok_list *list)
//{
	//printf("tokens:");
	//do {
		//if (list->type == symbol) {
			//printf(" %s", list->sym.c_str());
		//} else {
			//printf(" %c", list->type);
		//}
	//} while (list = list->next);
	//std::cout << '\n';
//}

void tok_print(struct tok_list *x)
{
	struct tok_list *list;
	printf("tokens:");
	while (list = lex_next()) {
		if (list->type == symbol) {
			printf(" %s", list->sym.c_str());
		} else {
			printf(" %c", list->type);
		}
	}
	std::cout << '\n';
}

struct tok_list *lex_next()
{
	if (next->type == end) return 0;
	return next = next->next;
}
void lex_reset()
{
	next = first;
}