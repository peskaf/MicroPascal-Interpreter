#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include <string>
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

	static bool IsLiteral(std::variant<Literal, std::shared_ptr<Callable>> value);
	static bool IsCallable(std::variant<Literal, std::shared_ptr<Callable>> value);

	void Define(Token name, VariableType type);
	void Define(Token name, Callable callable);

	std::variant<Literal, std::shared_ptr<Callable>>& Get(Token& name);
	Literal& GetLiteral(Token& name);
	std::shared_ptr<Callable>& GetCallable(Token& name);

	void Assign(Token& name, Literal value);

	std::shared_ptr<Environment> enclosing_env;

private:
	std::unordered_map<std::string, std::variant<Literal, std::shared_ptr<Callable>>> values;
};


class Callable {
public:
	Callable(std::shared_ptr<Stmt> m_body, std::vector<std::shared_ptr<Stmt>> m_declarations, std::vector<std::pair<Token, VariableType>> m_parameters, std::optional<VariableType> m_return_type);
	
	void PassArguments(std::vector<Literal> arguments, Token& callee);

	std::shared_ptr<Environment> local_env;
	std::vector<std::shared_ptr<Stmt>> declarations;
	std::vector<std::pair<Token, VariableType>> parameters;
	std::shared_ptr<Stmt> body;
	std::optional<VariableType> return_type;
};


#endif // !ENVIRONMENT_HPP