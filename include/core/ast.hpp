#ifndef __AST_H_
#define __AST_H_
#include <string>

enum nodetype {
	npair,
	nsym,
	nwrap,
	nnil
};

enum wraptype {
	wpromote,
	wdemote,
	wimmed
};

class ASTNode {
	public:
		virtual enum nodetype getType() = 0;
		virtual std::string str() = 0;
};

class ASTSymbol: public ASTNode {
	public:
		ASTSymbol(std::string s);
		enum nodetype getType();
		std::string str();
		std::string getSym();
	private:
		const std::string sym;
};

class ASTPair: public ASTNode {
	public:
		ASTPair(ASTNode *f, ASTNode *s);
		enum nodetype getType();
		std::string str();
		ASTNode *getFirst();
		ASTNode *getSecond();
	private:
		ASTNode *first;
		ASTNode *second;
};

class ASTWrap: public ASTNode {
	public:
		ASTWrap(ASTNode *i, enum wraptype wt);
		enum nodetype getType();
		std::string str();
		ASTNode *getInner();
		enum wraptype getWrapType();
	private:
		ASTNode *inner;
		enum wraptype type;
};

extern class ASTNil: public ASTNode {
	public:
		enum nodetype getType();
		std::string str();
} ast_nil;

#endif // __AST_H_