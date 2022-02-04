#include <variant>

#include "Interpreter.hpp"
#include "Error.hpp"

Literal Interpreter::Visit(BinaryExpr& binExpr)
{
	Literal left_value = binExpr.left->Accept(*this);
	Literal right_value = binExpr.right->Accept(*this);

	if (IsInt(left_value) && IsInt(right_value))
	{
		switch (binExpr.op.type)
		{
		case TokenType::PLUS:
			return std::get<int>(left_value) + std::get<int>(right_value);
		case TokenType::MINUS:
			return std::get<int>(left_value) - std::get<int>(right_value);
		case TokenType::MUL:
			return std::get<int>(left_value) * std::get<int>(right_value);
		case TokenType::DIV:
			if (std::get<int>(right_value) == 0)
			{
				Error::ThrowError(binExpr.op.line_num,"division by zero.");
			}
			return std::get<int>(left_value) / std::get<int>(right_value);
		case TokenType::GREATER_EQUAL:
			return std::get<int>(left_value) >= std::get<int>(right_value);
		case TokenType::GREATER:
			return std::get<int>(left_value) > std::get<int>(right_value);
		case TokenType::LESS_EQUAL:
			return std::get<int>(left_value) <= std::get<int>(right_value);
		case TokenType::LESS:
			return std::get<int>(left_value) < std::get<int>(right_value);
		default:
			break;
		}
	}

	if (IsString(left_value) && IsString(right_value) && binExpr.op.type == TokenType::PLUS) // string concat on + op
	{
		return std::get<std::string>(left_value) + std::get<std::string>(right_value);
	}

	if (left_value.index() == right_value.index()) // comparison only for same types
	{
		switch (binExpr.op.type)
		{
		case TokenType::EQUAL:
			return left_value == right_value;
		case TokenType::NOT_EQUAL:
			return left_value != right_value;
		default:
			break;
		}
	}

	Error::ThrowError(binExpr.op.line_num, "types incompatible with given operator.");
}

Literal Interpreter::Visit(LiteralExpr& litExpr)
{
	return litExpr.value;
}

Literal Interpreter::Visit(UnaryExpr& unExpr)
{
	Literal right_value = unExpr.right->Accept(*this);

	if (IsInt(right_value))
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
	}

	if (IsBool(right_value) && unExpr.op.type == TokenType::NOT)
	{
		return !std::get<bool>(unExpr.right->Accept(*this));
	}
	
	Error::ThrowError(unExpr.op.line_num, "type incompatible with given operator.");
}

Literal Interpreter::Visit(GroupingExpr& grExpr)
{
	return grExpr.expr->Accept(*this);
}

// neni finalni!!!
void Interpreter::Interpret(std::unique_ptr<Expr> expr)
{
	Literal result = expr->Accept(*this);
	if (IsInt(result))
	{
		std::cout << std::get<int>(result) << std::endl;
		return;
	}
	else if (IsString(result))
	{
		std::cout << std::get<std::string>(result) << std::endl;
		return;
	}
	else if (IsBool(result))
	{
		std::cout << (std::get<bool>(result) ? "true" : "false") << std::endl;
	}
}

// 1 .. int, 2 .. bool, 3 .. string
bool Interpreter::IsInt(Literal& lit)
{
	return lit.index() == 1;
}

bool Interpreter::IsBool(Literal& lit)
{
	return lit.index() == 2;
}

bool Interpreter::IsString(Literal& lit)
{
	return lit.index() == 3;
}