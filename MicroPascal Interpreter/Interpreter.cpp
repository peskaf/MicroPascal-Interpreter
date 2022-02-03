#include <variant>

#include "Interpreter.hpp"


Literal Interpreter::Visit(BinaryExpr& binExpr)
{
	switch (binExpr.op.type)
	{
	case TokenType::PLUS:
		return std::get<int>(binExpr.left->Accept(*this)) + std::get<int>(binExpr.right->Accept(*this)); // for int only, assumes no error
	case TokenType::MINUS:
		return std::get<int>(binExpr.left->Accept(*this)) - std::get<int>(binExpr.right->Accept(*this)); // for int only, assumes no error
	case TokenType::MUL:
		return std::get<int>(binExpr.left->Accept(*this)) * std::get<int>(binExpr.right->Accept(*this)); // for int only, assumes no error
	case TokenType::DIV:
		// TODO: zero division
		return std::get<int>(binExpr.left->Accept(*this)) / std::get<int>(binExpr.right->Accept(*this)); // for int only, assumes no error
	default:
		// sem neco dat
		break;
	}
	throw std::runtime_error("Some error.");
}

Literal Interpreter::Visit(LiteralExpr& litExpr)
{
	return std::get<int>(litExpr.value);
}

Literal Interpreter::Visit(UnaryExpr& unExpr)
{
	switch (unExpr.op.type)
	{
	case TokenType::MINUS:
		return -std::get<int>(unExpr.right->Accept(*this));
	case TokenType::PLUS:
		return std::get<int>(unExpr.right->Accept(*this));
	default:
		break;
	}
	throw std::runtime_error("Some error.");
}

Literal Interpreter::Visit(GroupingExpr& grExpr)
{
	return grExpr.expr->Accept(*this);
}