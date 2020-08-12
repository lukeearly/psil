#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "syntax/psil/lex.hpp"
#include "core/ast.hpp"

std::string expectedstr = "Expected ";

void parse_error(std::string desc)
{
	std::cerr << "ERROR: " << desc;
	exit(1);
}

void parse_warning(std::string desc)
{
	std::cerr << "Warning:" << desc;
	exit(1);
}

ASTNode *parse_list(ASTNode *term, enum tok_type til);

ASTNode *parse_unary()
{
	struct tok_list *tok;
	if (tok = lex_next()) {
		switch (tok->type) {
		case left_paren:
			return parse_list(&ast_nil, right_paren);
		case left_chev:
			return parse_list(parse_unary(), right_chev);

		case right_paren:
			parse_error("Mismatched right parenthesis, or incomplete unary operator.\n");
		case right_chev:
			parse_error("Mismatched right chevron, or incomplete unary/<> operator.\n");
		case dot:
			parse_error("The cons operator (.) is not yet implemented! Use <a b> instead.\n");

		case dollar:
			return new ASTWrap(parse_unary(), wpromote);
		case colon:
			return new ASTWrap(parse_unary(), wdemote);
		case hash:
			return new ASTWrap(parse_unary(), wimmed);
		//case at:
			//return new ASTWrap(parse_unary(), emac);
			//break;
		case symbol:
			return new ASTSymbol(tok->sym);
		}
	}
	parse_error("Incomplete unary/<> operator.\n");
}

/*
ASTNode *parse_macro()
{
	struct tok_list *tok;
	ASTNode *macro;
	ASTNode *arg;
	if (tok = lex_next()) {
		switch (tok->type) {
		case left_paren:
			macro = parse_paren();
			break;
		case left_chev:
			parse_error("Chevron not allowed as macro. Use a symbol or parens.");
			break;

		case right_paren:
			parse_error("Mismatched right parenthesis.");
			break;
		case right_chev:
			parse_error("Mismatched right chevron.");
			break;

		case dot:
			parse_error("Cons not allowed as macro. Use a symbol or parens.");
			break;

		case dollar:
			parse_error("Code expansion not allowed as macro. Use a symbol or parens.");
			//macro = new ASTWrap(parse_unary(), wpromote);
			break;
		case colon:
			parse_error("Code literal not allowed as macro. Use a symbol or parens.");
			//macro = new ASTWrap(parse_unary(), wdemote);
			break;
		case hash:
			parse_warning("used the result of a macro as a macro identifier.\n");
			inner = new ASTPair(inner, new ASTMacro(new ASTSymbol("Num"), parse_unary()));
			break;
		case at:
			parse_error("Spliced expression not allowed as macro. Use a symbol or parens.");
			break;
		case symbol:
			macro = new ASTSymbol(tok->sym);
			break;
		default:
			parse_error("Unexpected token.");
		}
		if (tok = lex_next()) {
			switch (tok->type) {
			case left_paren:
				arg = parse_paren();
				break;
			case left_chev:
				arg = parse_chev();
				break;

			case right_paren:
				parse_error("Mismatched right parenthesis.");
				break;
			case right_chev:
				parse_error("Mismatched right chevron.");
				break;

			case dot:
				parse_error("Cons not allowed as macro. Use a symbol or parens.");
				break;

			case dollar:
				arg = new ASTWrap(parse_unary(), wpromote);
				break;
			case colon:
				arg = new ASTWrap(parse_unary(), wdemote);
				break;
			case hash:
				parse_warning("Used the result of a macro as a macro arg.\n");
				arg = parse_macro();
				break;
			case at:
				parse_error("Spliced expression not allowed as macro arg.");
				break;
			case symbol:
				arg = new ASTSymbol(tok->sym);
				break;
			default:
				parse_error("Unexpected token.");
			}
		}
	}
	if (macro && arg)
	{
		return new ASTMacro()
	}
	parse_error("Incomplete macro invocation\n");
}
*/

ASTNode *parse_list(ASTNode *term, enum tok_type til)
{
	struct tok_list *tok;
	ASTNode *inner = term;
	while (tok = lex_next()) {
		if (tok->type == til) {
			return inner;
		}
		switch (tok->type) {
		case left_paren:
			inner = new ASTPair(inner, parse_list(&ast_nil, right_paren));
			break;
		case left_chev:
			inner = new ASTPair(inner, parse_list(parse_unary(), right_chev));
			break;

		case right_paren:
			parse_error("Mismatched right parenthesis.\n");
			break;
		case right_chev:
			parse_error("Mismatched right chevron.\n");
			break;

		case dot:
			parse_error("The cons operator (.) is not yet implemented! Use <a b> instead.\n");
			break;

		case dollar:
			inner = new ASTPair(inner, new ASTWrap(parse_unary(), wpromote));
			break;
		case colon:
			inner = new ASTPair(inner, new ASTWrap(parse_unary(), wdemote));
			break;
		case hash:
			inner = new ASTPair(inner, new ASTWrap(parse_unary(), wimmed));
			break;
		//case at:
		case symbol:
			inner = new ASTPair(inner, new ASTSymbol(tok->sym));
		}
	}
	parse_error(expectedstr + ((char) til) + "\n");
}

void xml_repl()
{
	std::string line;
	while (1) {
		std::cout << "\nλ ";
		std::getline(std::cin, line);
		std::stringstream stream(line);
		tok_print(lex_def(stream));
		lex_reset();
		ASTNode *tree = parse_unary();
		std::cout << tree->str();
	}
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		struct tok_list *list = lex_def(std::cin);
		tok_print(list);
		lex_reset();
		ASTNode *tree = parse_unary();
		std::cout << tree->str();
	} else {
		std::string arg(argv[1]);
		if (arg == "--repl") xml_repl();
		struct tok_list *list = lex_def(*new std::ifstream(arg));
		tok_print(list);
		lex_reset();
		ASTNode *tree = parse_unary();
		std::cout << tree->str();
	}
}