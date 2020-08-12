#include "core/ast.hpp"
#include <string>

ASTPair::ASTPair(ASTNode *f, ASTNode *s): first(f), second(s) {}
enum nodetype ASTPair::getType()
{
	return npair;
}
std::string ASTPair::str()
{
	return "<pair>" + first->str() + second->str() + "</pair>";
}
ASTNode *ASTPair::getFirst()
{
	return first;
}
ASTNode *ASTPair::getSecond()
{
	return second;
}

ASTSymbol::ASTSymbol(std::string s): sym(s) {}
enum nodetype ASTSymbol::getType()
{
	return nsym;
}
std::string ASTSymbol::str()
{
	return "<sym>" + sym + "</sym>";
}
std::string ASTSymbol::getSym()
{
	return sym;
}

ASTWrap::ASTWrap(ASTNode *i, enum wraptype wt): inner(i), type(wt) {}
enum nodetype ASTWrap::getType()
{
	return nwrap;
}
std::string evalToStr(enum wraptype et)
{
	switch (et) {
	case wpromote:
		return "up";
	case wdemote:
		return "down";
	case wimmed:
		return "immed";
	}
}
std::string ASTWrap::str()
{
	return "<wrap type=\"" + evalToStr(type) + "\">" + inner->str() + "</wrap>";
}
ASTNode* ASTWrap::getInner()
{
	return inner;
}
enum wraptype ASTWrap::getWrapType()
{
	return type;
}

enum nodetype ASTNil::getType()
{
	return nnil;
}
std::string ASTNil::str()
{
	return "<nil/>";
}

ASTNil ast_nil;