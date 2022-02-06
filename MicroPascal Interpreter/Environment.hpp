#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <string>
#include <typeindex>
#include <variant>
#include <memory>
#include <vector>

#include "Stmt.hpp"
#include "Token.hpp"

class Callable;

class Environment
{
public:
	Environment();
	Environment(std::shared_ptr<Environment> m_enclosing_env);

	void Define(Token name, VariableType type);
	void Define(Token name, Callable callable);
	std::variant<Literal, std::shared_ptr<Callable>> Get(Token name);
	Literal GetLiteral(Token name);
	std::shared_ptr<Callable> GetCallable(Token name);
	void Assign(Token name, Literal value);

	std::shared_ptr<Environment> enclosing_env;

	std::map<std::string, std::variant<Literal, std::shared_ptr<Callable>>> values; // pak dat zpet do private!!
private:
	
};


class Callable {
public:
	Callable(std::shared_ptr<Environment> parent_env, std::shared_ptr<Stmt> m_body, std::vector<std::pair<std::string, VariableType>> m_parameters, std::optional<VariableType> m_return_type);
	
	void PassArguments(std::vector<Literal>);

	std::shared_ptr<Environment> local_env;
	std::shared_ptr<Stmt> body;
	std::optional<VariableType> return_type;

private:
	std::vector<std::pair<std::string, VariableType>> parameters;
};


#endif // !ENVIRONMENT_HPP