#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Expr.hpp"

class Interpreter : public Visitor
{
public:
	Literal Visit(BinaryExpr& binExpr) override;

	Literal Visit(LiteralExpr& litExpr) override;

	Literal Visit(UnaryExpr& unExpr) override;

	Literal Visit(GroupingExpr& grExpr) override;
};


#endif // !INTERPRETER_HPP
