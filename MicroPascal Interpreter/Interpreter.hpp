#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <memory>

#include "Expr.hpp"

class Interpreter : public Visitor
{
public:
	Literal Visit(BinaryExpr& binExpr) override;

	Literal Visit(LiteralExpr& litExpr) override;

	Literal Visit(UnaryExpr& unExpr) override;

	Literal Visit(GroupingExpr& grExpr) override;

	void Interpret(std::unique_ptr<Expr> expr);

private:
	bool IsInt(Literal& lit);

	bool IsString(Literal& lit);

	bool IsBool(Literal& lit);
};

#endif // !INTERPRETER_HPP