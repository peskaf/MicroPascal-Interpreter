#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <string>
#include <typeindex>


#include "Token.hpp"

class Environment
{
public:
	void Define(Token name, int type_id);
	Literal Get(Token name);
	void Assign(Token name, Literal value);

private:
	std::map<std::string, Literal> values;
};


#endif // !ENVIRONMENT_HPP