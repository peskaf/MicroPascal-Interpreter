#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <variant>
#include <iostream>

#include "TokenType.hpp"

using Literal = std::variant<std::nullptr_t, int, bool, std::string>;

enum class VariableType
{
	INTEGER,
	BOOL,
	STRING
};

class Token
{
public:
	Token(TokenType m_type, std::string m_lexeme, Literal m_lit, int m_line_num)
		: type(m_type), lexeme(m_lexeme), lit(m_lit), line_num(m_line_num) {}

	void Print() // for current debug
	{
		std::cout << type << " " << lexeme << " " << line_num << std::endl;
	}

	TokenType type;
	std::string lexeme;
	Literal lit;
	int line_num; // for error handling
};

#endif // !TOKEN_HPP

