#include <variant>

#include "Interpreter.hpp"


Literal Interpreter::Visit(BinaryExpr& binExpr)
{
	Literal left_value = binExpr.left->Accept(*this);
	Literal right_value = binExpr.right->Accept(*this);
	std::cout << left_value.index() << " " << right_value.index() << std::endl;
	switch (binExpr.op.type)
	{
	case TokenType::PLUS:
		if (left_value.index() == 1 && right_value.index() == 1) // 1 .. int, 2 .. bool, 3 .. string
		{
			return std::get<int>(left_value) + std::get<int>(right_value);
		}
		if (left_value.index() == 3 && right_value.index() == 3)
		{
			return std::get<std::string>(left_value) + std::get<std::string>(right_value);
		}
		break;
	case TokenType::MINUS:
		if (left_value.index() == 1 && right_value.index() == 1)
		{
			return std::get<int>(left_value) - std::get<int>(right_value);
		}
		break;
	case TokenType::MUL:
		if (left_value.index() == 1 && right_value.index() == 1)
		{
			return std::get<int>(left_value) * std::get<int>(right_value);
		}
		break;
	case TokenType::DIV:
		if (left_value.index() == 1 && right_value.index() == 1)
		{
			if (std::get<int>(right_value) == 0)
			{
				throw std::runtime_error("Division by zero.");
			}
			return std::get<int>(left_value) / std::get<int>(right_value);
		}
		break;
	case TokenType::EQUAL:
		if (left_value.index() == right_value.index()) // same types only
		{
			return left_value == right_value;
		}
		break;
	case TokenType::NOT_EQUAL:
		if (left_value.index() == right_value.index()) // same types only
		{
			return left_value != right_value;
		}
		break;
	case TokenType::GREATER_EQUAL:
		if (left_value.index() == 1 && right_value.index() == 1) // only ints
		{
			return std::get<int>(left_value) >= std::get<int>(right_value);
		}
		break;
	case TokenType::LESS_EQUAL:
		if (left_value.index() == 1 && right_value.index() == 1) // only ints
		{
			return std::get<int>(left_value) <= std::get<int>(right_value);
		}
		break;
	case TokenType::GREATER:
		if (left_value.index() == 1 && right_value.index() == 1) // only ints
		{
			return std::get<int>(left_value) > std::get<int>(right_value);
		}
		break;
	case TokenType::LESS:
		if (left_value.index() == 1 && right_value.index() == 1) // only ints
		{
			return std::get<int>(left_value) < std::get<int>(right_value);
		}
		break;
	default:
		throw std::runtime_error("Incompatible types or invalid operator.");
		break;
	}
}

Literal Interpreter::Visit(LiteralExpr& litExpr)
{
	return litExpr.value;
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