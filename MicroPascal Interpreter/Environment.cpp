#include "Environment.hpp"
#include "Error.hpp"

void Environment::Define(Token name, int type_id)
{
	if (values.find(name.lexeme) == values.end()) // it is not there yet
	{
		// Pascal assigns rubbish to variables -> here, zero assignment like C#
		switch (type_id)
		{
		case 1: // int
			values[name.lexeme] = 0;
			return;
		case 2: // bool
			values[name.lexeme] = false;
			return;
		case 3: // string
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
	if (values.find(name.lexeme) != values.end()) // it is there
	{
		return values[name.lexeme];
	}
	Error::ThrowError(name.line_num, "identifier not found.");
}

void Environment::Assign(Token name, Literal value)
{
	if (values.find(name.lexeme) != values.end()) // it is already there
	{
		if (values[name.lexeme].index() == value.index()) // types have to be the same
		{
			values[name.lexeme] = value;
			return;
		}
		Error::ThrowError(name.line_num, "incompatible types.");
	}
	Error::ThrowError(name.line_num, "identifier not found.");
}
