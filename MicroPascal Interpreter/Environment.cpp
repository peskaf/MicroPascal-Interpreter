#include "Environment.hpp"
#include "Error.hpp"

Environment::Environment() : enclosing_env(nullptr) {};

Environment::Environment(std::unique_ptr<Environment> m_enclosing_env) : enclosing_env(std::move(m_enclosing_env)) {}

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
			values[name.lexeme] = "";
			return;
		default:
			break;
		}
	}
	Error::ThrowError(name.line_num, "duplicate identifier.");
}

Literal Environment::Get(Token name)
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

void Environment::Assign(Token name, Literal value)
{
	// try to assign in current env
	if (values.find(name.lexeme) != values.end()) // name exists in current env
	{
		if (values[name.lexeme].index() == value.index()) // types have to be the same
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
