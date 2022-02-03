#include <variant>

#include "Expr.hpp"

BinaryExpr::BinaryExpr(std::unique_ptr<Expr> m_left, std::unique_ptr<Expr> m_right, Token& m_op) : left(std::move(m_left)), right(std::move(m_right)), op(m_op) {};
	
Literal BinaryExpr::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
};


UnaryExpr::UnaryExpr(std::unique_ptr<Expr> m_right, Token& m_op) : right(std::move(m_right)), op(m_op) {};

Literal UnaryExpr::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
};


LiteralExpr::LiteralExpr(Literal m_value) : value(m_value) {};

Literal LiteralExpr::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
};


GroupingExpr::GroupingExpr(std::unique_ptr<Expr> m_expr) : expr(std::move(m_expr)) {};

Literal GroupingExpr::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
};
