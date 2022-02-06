#include "Environment.hpp"
#include "Error.hpp"

Environment::Environment() : enclosing_env(nullptr) {};

Environment::Environment(std::shared_ptr<Environment> m_enclosing_env) : enclosing_env(std::move(m_enclosing_env)) {}

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
			values[name.lexeme] = std::string();
			return;
		default:
			Error::ThrowError(name.line_num, "invalid type.");
			break;
		}
	}
	Error::ThrowError(name.line_num, "duplicate identifier.");
}

void Environment::Define(Token name, Callable callable)
{
	if (values.find(name.lexeme) == values.end()) // it is not there yet
	{
		values[name.lexeme] = std::make_shared<Callable>(callable);
		return;
	}
	Error::ThrowError(name.line_num, "duplicate identifier.");
}

std::variant<Literal, std::shared_ptr<Callable>> Environment::Get(Token name)
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

	Error::ThrowError(name.line_num, "identifier not found.");
}

Literal Environment::GetLiteral(Token name)
{
	auto&& value = Get(name);
	if (value.index() == 0) // literal
	{
		return std::get<Literal>(value);
	}
	Error::ThrowError(name.line_num, "literal expected.");
}

std::shared_ptr<Callable> Environment::GetCallable(Token name)
{
	auto&& value = Get(name);
	if (value.index() == 1) // callable
	{
		return std::get<std::shared_ptr<Callable>>(value);
	}
	Error::ThrowError(name.line_num, "callable expected.");
}

void Environment::Assign(Token name, Literal value)
{
	// try to assign in current env
	if (values.find(name.lexeme) != values.end()) // name exists in current env
	{
		if (GetLiteral(name).index() == value.index()) // types have to be the same
		{
			values[name.lexeme] = value;
			return;
		}
		Error::ThrowError(name.line_num, "incompatible types.");
	}

	// try to assign in enclosing env
	if (enclosing_env != nullptr)
	{
		enclosing_env->Assign(name, value);
		return;
	}

	Error::ThrowError(name.line_num, "identifier not found.");
}

Callable::Callable(std::shared_ptr<Environment> parent_env, std::shared_ptr<Stmt> m_body, std::vector<std::pair<Token, VariableType>> m_parameters, std::optional<VariableType> m_return_type)
	: local_env(std::make_shared<Environment>(parent_env)), body(std::move(m_body)), parameters(m_parameters), return_type(m_return_type) {};

void Callable::PassArguments(std::vector<Literal> arguments, Token& callee)
{
	// arity check
	if (arguments.size() != parameters.size()) // different number of args
	{
		Error::ThrowError(callee.line_num, "invalid number of arguments.");
		return;
	}

	// type check
	for (size_t i = 0; i < arguments.size(); i++)
	{
		if (arguments[i].index() == 1 && parameters[i].second != VariableType::INTEGER ||
			arguments[i].index() == 2 && parameters[i].second != VariableType::BOOL ||
			arguments[i].index() == 3 && parameters[i].second != VariableType::STRING) // types does not match
		{
			Error::ThrowError(callee.line_num, "incompatible type for argument.");
		}
	}

	// assign values to variables in local env
	for (size_t i = 0; i < arguments.size(); i++)
	{	
		local_env->Assign(parameters[i].first, arguments[i]);
	}
}
