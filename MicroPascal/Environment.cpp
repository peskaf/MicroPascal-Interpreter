#include "Environment.hpp"
#include "Error.hpp"

Environment::Environment() : enclosing_env(nullptr) {};

Environment::Environment(std::shared_ptr<Environment> m_enclosing_env) : enclosing_env(std::move(m_enclosing_env)) {}


bool Environment::IsLiteral(std::variant<Literal, std::shared_ptr<Callable>> value)
{
	return value.index() == 0; // 0 -> index in variant
}

bool Environment::IsCallable(std::variant<Literal, std::shared_ptr<Callable>> value)
{
	return value.index() == 1; // 1 -> index in variant
}


void Environment::Define(Token name, VariableType type)
{
	if (values.find(name.lexeme) == values.end()) // it is not there yet
	{
		// Pascal assigns rubbish to variables -> here, zero assignment like in C#
		switch (type)
		{
		case VariableType::INTEGER:
			values[name.lexeme] = 0;
			return;
		case VariableType::BOOL:
			values[name.lexeme] = false;
			return;
		case VariableType::STRING:
			values[name.lexeme] = std::string(); // note: "" does not work -> gets evaluated to 'false' somehow in some cases
			return;
		default:
			throw Error(name.line_num, "invalid type.");
		}
	}
	throw Error(name.line_num, "duplicate identifier.");
}

void Environment::Define(Token name, Callable callable)
{
	if (values.find(name.lexeme) == values.end()) // it is not there yet
	{
		values[name.lexeme] = std::make_shared<Callable>(callable);
		return;
	}
	throw Error(name.line_num, "duplicate identifier.");
}


std::variant<Literal, std::shared_ptr<Callable>>& Environment::Get(Token& name)
{
	// look for variable in current scope
	if (values.find(name.lexeme) != values.end()) // name exists in current env
	{
		return values[name.lexeme];
	}

	// look for variable in the enclosing scope
	if (enclosing_env != nullptr)
	{
		return enclosing_env->Get(name);
	}

	throw Error(name.line_num, "identifier not found.");
}

Literal& Environment::GetLiteral(Token& name)
{
	auto&& value = Get(name);

	if (IsLiteral(value)) 
	{
		return std::get<Literal>(value);
	}
	
	throw Error(name.line_num, "literal expected.");
}

std::shared_ptr<Callable>& Environment::GetCallable(Token& name)
{
	auto&& value = Get(name);

	if (IsCallable(value))
	{
		return std::get<std::shared_ptr<Callable>>(value);
	}

	// allows recursive calls (func has literal with same ID as return variable -> it looks for callable in encl. env. to find itself as callable)
	if (enclosing_env != nullptr)
	{
		return enclosing_env->GetCallable(name);
	}

	throw Error(name.line_num, "callable expected.");
}


void Environment::Assign(Token& name, Literal value)
{
	// try to assign in current env
	if (values.find(name.lexeme) != values.end()) // name exists in current env
	{
		if (GetLiteral(name).index() == value.index()) // types have to be the same
		{
			values[name.lexeme] = value;
			return;
		}
		throw Error(name.line_num, "incompatible types.");
	}

	// try to assign in enclosing env
	if (enclosing_env != nullptr)
	{
		enclosing_env->Assign(name, value);
		return;
	}

	throw Error(name.line_num, "identifier not found.");
}



Callable::Callable(std::shared_ptr<Stmt> m_body, std::vector<std::shared_ptr<Stmt>> m_declarations, std::vector<std::pair<Token, VariableType>> m_parameters, std::optional<VariableType> m_return_type)
	: body(std::move(m_body)), declarations(std::move(m_declarations)), parameters(m_parameters), return_type(m_return_type) {};


void Callable::PassArguments(std::vector<Literal> arguments, Token& callee)
{
	// arity check
	if (arguments.size() != parameters.size()) // different number of args
	{
		throw Error(callee.line_num, "invalid number of arguments.");
		return;
	}

	// type check
	for (size_t i = 0; i < arguments.size(); i++)
	{
		// 1 .. int, 2 .. bool, 3 .. string
		if (((arguments[i].index() == 1) && parameters[i].second != VariableType::INTEGER) ||
			((arguments[i].index() == 2) && parameters[i].second != VariableType::BOOL) ||
			((arguments[i].index() == 3) && parameters[i].second != VariableType::STRING)) // types do not match
		{
			throw Error(callee.line_num, "incompatible type for argument.");
		}
	}

	// assign values to variables in local env
	for (size_t i = 0; i < arguments.size(); i++)
	{	
		local_env->Assign(parameters[i].first, arguments[i]);
	}
}
