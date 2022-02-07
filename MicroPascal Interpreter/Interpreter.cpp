#include <variant>

#include "Interpreter.hpp"
#include "Error.hpp"

Interpreter::Interpreter() : global_env(std::make_shared<Environment>()), current_env(global_env) {};

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
	// function without parameters
	if (current_env->Get(varExpr.token).index() == 1) 
	{
		auto&& callable = current_env->GetCallable(varExpr.token);

		// move to local env for execution while remembering the previous one
		auto prev_env = current_env;
		current_env = callable->local_env;

		// body execution
		callable->body->Accept(*this);

		// go back to previous environment (caller's one)
		current_env = prev_env;

		return callable->local_env->GetLiteral(varExpr.token);
	}
	// literal
	return current_env->GetLiteral(varExpr.token);
}

void Interpreter::Visit(WritelnStmt& writelnStmt)
{
	for (auto&& expr : writelnStmt.exprs) // evaluate all expressions inside writeln stmt
	{
		Literal to_print = expr->Accept(*this);
		std::cout << LitToString(to_print);
	}

	std::cout << std::endl; // new line and flush
}

void Interpreter::Visit(EmptyStmt& emptyStmt) {} // do nothing

void Interpreter::Visit(VarDeclStmt& varDeclStmt) // define all variables
{
	for (auto&& [type, tok_vec] : varDeclStmt.variables)
	{
		for (auto&& tok : tok_vec)
		{
			current_env->Define(tok, type);
		}
	}
}

Literal Interpreter::Visit(FunctionCallExpr& funcCallExpr)
{
	stack_count++;
	CheckStackOverflow();

	std::vector<Literal> arguments;

	// evaluate all expressions to literals
	for (auto&& expr : funcCallExpr.exprs)
	{
		arguments.push_back(expr->Accept(*this));
	}

	// (get callable by id) interpret all declarations in function object
	auto&& callable = current_env->GetCallable(funcCallExpr.id_token);

	// remember current env
	std::shared_ptr<Environment> prev_env = current_env;

	std::shared_ptr<Environment> local_env = std::make_shared<Environment>(current_env);
	callable->local_env = local_env; // pass local env to callable
	current_env = local_env;

	for (auto&& declStmt : callable->declarations)
	{
		declStmt->Accept(*this);
	}

	// define parameters variables in local env
	for (auto&& [var, type] : callable->parameters)
	{
		current_env->Define(var, type);
	}

	// define variable that will serve as return (value in it will be returned), id same as func id
	current_env->Define(funcCallExpr.id_token, callable->return_type.value());

	// pass arguments to callable -> arity, type check and arguments assignment happens over there
	callable->PassArguments(arguments, funcCallExpr.id_token);

	// body execution
	callable->body->Accept(*this);


	// return value -> need to get it before exiting enviornment
	Literal return_value = current_env->GetLiteral(funcCallExpr.id_token);

	// go back to previous environment (caller's one)
	current_env = prev_env;

	// return 
	stack_count--;
	return return_value;
}

void Interpreter::Visit(FuncDeclStmt& funcDeclStmt)
{
	// make callable 
	auto&& callable = Callable(std::move(funcDeclStmt.body), std::move(funcDeclStmt.decl_stmts), funcDeclStmt.parameters, funcDeclStmt.return_type);

	// define function by id in current env
	current_env->Define(funcDeclStmt.id_token, std::move(callable));
}

void Interpreter::Visit(ProcDeclStmt& procDeclStmt)
{
	// make callable 
	auto&& callable = Callable(std::move(procDeclStmt.body), std::move(procDeclStmt.decl_stmts), procDeclStmt.parameters, std::nullopt);

	// define procedure by id in current env
	current_env->Define(procDeclStmt.id_token, std::move(callable));
}

void Interpreter::Visit(ProcedureCallStmt& procCallStmt)
{
	stack_count++;
	CheckStackOverflow();

	std::vector<Literal> arguments;

	// evaluate all expressions to literals
	for (auto&& expr : procCallStmt.exprs)
	{
		arguments.push_back(expr->Accept(*this));
	}

	// (get callable by id) interpret all declarations in function object
	auto&& callable = current_env->GetCallable(procCallStmt.id_token);

	// remember current env
	std::shared_ptr<Environment> prev_env = current_env;

	std::shared_ptr<Environment> local_env = std::make_shared<Environment>(current_env);
	callable->local_env = local_env; // pass local env to callable
	current_env = local_env;

	for (auto&& declStmt : callable->declarations)
	{
		declStmt->Accept(*this);
	}

	// define parameters variables in local env
	for (auto&& [var, type] : callable->parameters)
	{
		current_env->Define(var, type);
	}

	// pass arguments to callable -> arity, type check and arguments assignment happens over there
	callable->PassArguments(arguments, procCallStmt.id_token);

	// body execution
	callable->body->Accept(*this);

	// go back to previous environment (caller's one)
	current_env = prev_env;

	stack_count--;
}

void Interpreter::Visit(ProgramStmt& programStmt)
{
	// first interpret all declarations
	for (auto&& declStmt : programStmt.decl_stmts)
	{
		declStmt->Accept(*this);
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

void Interpreter::Visit(AssignmentStmt& assignmentStmt)
{
	current_env->Assign(assignmentStmt.token, assignmentStmt.value->Accept(*this));
}

void Interpreter::Visit(IfStmt& ifStmt)
{
	Literal condition_value = ifStmt.condition->Accept(*this);

	if (IsBool(condition_value))
	{
		if (std::get<bool>(condition_value))
		{
			ifStmt.then_branch->Accept(*this);
		}
		else if (ifStmt.else_branch.has_value())
		{
			ifStmt.else_branch.value()->Accept(*this);
		}
		return; 
	}
	Error::ThrowError(ifStmt.token.line_num, "expected boolean value.");
}

void Interpreter::Visit(WhileStmt& whileStmt)
{
	Literal condition_value = whileStmt.condition->Accept(*this);

	if (IsBool(condition_value))
	{
		while (std::get<bool>(whileStmt.condition->Accept(*this))) // need to Accept visitor like this over and over because of environment change
		{
			whileStmt.body->Accept(*this);
		}
		return;
	}
	Error::ThrowError(whileStmt.token.line_num, "expected boolean value.");
}

void Interpreter::Visit(ForStmt& forStmt)
{
	// note: according to Free Pascal Compiler version 3.0.2, expression_value is evaluated BEFORE initial value is assigned
	Literal expression_value = forStmt.expression->Accept(*this); // value that we count to/downto

	forStmt.assignment->Accept(*this); // assign init value of iterator variable

	Literal initial_value = current_env->GetLiteral(forStmt.id_token);

	if (IsInt(expression_value) && IsInt(initial_value))
	{
		if (forStmt.increment)
		{
			while (std::get<int>(current_env->GetLiteral(forStmt.id_token)) <= std::get<int>(expression_value))
			{
				forStmt.body->Accept(*this);
				current_env->Assign(forStmt.id_token, std::get<int>(current_env->GetLiteral(forStmt.id_token)) + 1);
			}
			return;
		}
		else // decrement
		{
			while (std::get<int>(current_env->GetLiteral(forStmt.id_token)) >= std::get<int>(expression_value))
			{
				forStmt.body->Accept(*this);
				current_env->Assign(forStmt.id_token, std::get<int>(current_env->GetLiteral(forStmt.id_token)) - 1);
			}
			return;
		}
	}
	Error::ThrowError(forStmt.for_token.line_num, "expected integer value.");
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

void Interpreter::CheckStackOverflow()
{
	if (stack_count > max_stack_count)
	{
		Error::ThrowError(0, "stack overflow.");
	}
}
