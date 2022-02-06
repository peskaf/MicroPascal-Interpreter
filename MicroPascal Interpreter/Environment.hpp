#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <string>
#include <typeindex>


#include "Token.hpp"

class Environment
{
public:
	Environment();
	Environment(std::shared_ptr<Environment> m_enclosing_env);

	void Define(Token name, VariableType type);
	Literal Get(Token name);
	void Assign(Token name, Literal value);

private:
	std::map<std::string, Literal> values;
	std::shared_ptr<Environment> enclosing_env;
};


#endif // !ENVIRONMENT_HPP