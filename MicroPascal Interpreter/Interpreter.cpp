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
			return -std::get<int>(right_value);
		case TokenType::PLUS:
			return std::get<int>(right_value);
		default:
			break;
		}
	}

	if (IsBool(right_value) && unExpr.op.type == TokenType::NOT)
	{
		return !std::get<bool>(right_value);
	}
	
	Error::ThrowError(unExpr.op.line_num, "type incompatible with given operator.");
}

Literal Interpreter::Visit(GroupingExpr& grExpr)
{
	return grExpr.expr->Accept(*this);
}

Literal Interpreter::Visit(VariableExpr& varExpr)
{
	return env.Get(varExpr.token);
}

void Interpreter::Visit(WritelnStmt& writelnStmt)
{
	if (!writelnStmt.exprs.has_value()) // empty -> writeln()
	{
		std::cout << std::endl;
		return;
	}

	for (auto&& expr : writelnStmt.exprs.value()) // evaluate all expressions inside writeln stmt
	{
		Literal to_print = expr->Accept(*this);
		std::cout << LitToString(to_print);
	}

	std::cout << std::endl; // new line and flush
}

void Interpreter::Visit(EmptyStmt& emptyStmt) {} // do nothing

void Interpreter::Visit(VarDeclStmt& varDeclStmt) // define all variables
{
	for (auto&& [type_id, tok_vec] : varDeclStmt.variables)
	{
		for (auto&& tok : tok_vec)
		{
			env.Define(tok, type_id);
		}
	}
}

void Interpreter::Visit(ProgramStmt& programStmt)
{
	// first interpret all declarations
	if (programStmt.decl_stmts.has_value()) // there are some decl
	{
		for (auto&& declStmt : programStmt.decl_stmts.value())
		{
			declStmt->Accept(*this);
		}
	}

	// then compound statement
	programStmt.stmt->Accept(*this); 
}

void Interpreter::Visit(CompoundStmt& compoundStmt)
{
	for (auto&& stmt : compoundStmt.statements)
	{
		stmt->Accept(*this);
	}
}

void Interpreter::Interpret(std::unique_ptr<Stmt> stmt)
{
	stmt->Accept(*this);
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

std::string Interpreter::LitToString(Literal& lit)
{
	if (IsBool(lit))
	{
		return (std::get<bool>(lit) ? "true" : "false");
	}
	if (IsInt(lit))
	{
		return std::to_string(std::get<int>(lit));
	}
	if (IsString(lit))
	{
		return std::get<std::string>(lit);
	}

	// should be unreachable -> will delete nullptr in literal variant
	Error::ThrowError(0, "invalid literal value."); // TODO: jeste poresit
}