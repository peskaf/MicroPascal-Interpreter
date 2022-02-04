#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>

#include "Token.hpp"

class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class GroupingExpr;

class Visitor
{
public:
	virtual Literal Visit(BinaryExpr& binExpr) = 0;
	virtual Literal Visit(UnaryExpr& unExpr) = 0;
	virtual Literal Visit(LiteralExpr& litExpr) = 0;
	virtual Literal Visit(GroupingExpr& grExpr) = 0;
};

class Expr
{
public:
	// virtual ~Expr();
	virtual Literal Accept(Visitor& visitor) = 0;
};

class BinaryExpr : public Expr
{
public:
	BinaryExpr(std::unique_ptr<Expr> m_left, std::unique_ptr<Expr> m_right, Token& m_op);

	Literal Accept(Visitor& visitor) override;

	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
	Token op;
};

class UnaryExpr : public Expr
{
public:
	UnaryExpr(std::unique_ptr<Expr> m_right, Token& m_op);

	Literal Accept(Visitor& visitor) override;

	std::unique_ptr<Expr> right;
	Token op;
};

class LiteralExpr : public Expr
{
public:
	LiteralExpr(Literal m_value);

	Literal Accept(Visitor& visitor) override;

	Literal value;
};

class GroupingExpr : public Expr // ( ... )
{
public:
	GroupingExpr(std::unique_ptr<Expr> m_expr);

	Literal Accept(Visitor& visitor) override;

	std::unique_ptr<Expr> expr;
};

#endif // !EXPR_HPP