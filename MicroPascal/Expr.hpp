#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>
#include <vector>

#include "Token.hpp"

class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class GroupingExpr;
class VariableExpr;
class FunctionCallExpr;

class VisitorExpr
{
public:
	virtual Literal Visit(BinaryExpr& binExpr) = 0;
	virtual Literal Visit(UnaryExpr& unExpr) = 0;
	virtual Literal Visit(LiteralExpr& litExpr) = 0;
	virtual Literal Visit(GroupingExpr& grExpr) = 0;
	virtual Literal Visit(VariableExpr& varExpr) = 0;
	virtual Literal Visit(FunctionCallExpr& funcCallExpr) = 0;
};


class Expr
{
public:
	virtual ~Expr() {};

	virtual Literal Accept(VisitorExpr& visitor) = 0;
};

class BinaryExpr : public Expr
{
public:
	BinaryExpr(std::unique_ptr<Expr> m_left, std::unique_ptr<Expr> m_right, Token& m_op);

	Literal Accept(VisitorExpr& visitor) override;

	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
	Token op;
};

class UnaryExpr : public Expr
{
public:
	UnaryExpr(std::unique_ptr<Expr> m_right, Token& m_op);

	Literal Accept(VisitorExpr& visitor) override;

	std::unique_ptr<Expr> right;
	Token op;
};

class LiteralExpr : public Expr
{
public:
	LiteralExpr(Literal m_value);

	Literal Accept(VisitorExpr& visitor) override;

	Literal value;
};

class GroupingExpr : public Expr
{
public:
	GroupingExpr(std::unique_ptr<Expr> m_expr);

	Literal Accept(VisitorExpr& visitor) override;

	std::unique_ptr<Expr> expr;
};

class VariableExpr : public Expr
{
public:
	VariableExpr(Token m_token);

	Literal Accept(VisitorExpr& visitor) override;

	Token token;
};

class FunctionCallExpr : public Expr
{
public:
	FunctionCallExpr(std::vector<std::unique_ptr<Expr>> m_exprs, Token m_id_token);

	Literal Accept(VisitorExpr& visitor) override;

	std::vector<std::unique_ptr<Expr>> exprs;
	Token id_token;
};

#endif // !EXPR_HPP