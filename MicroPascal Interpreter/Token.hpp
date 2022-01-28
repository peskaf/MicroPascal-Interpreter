#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <variant>

#include "TokenType.hpp"

using Literal = std::variant<std::nullptr_t, int, bool, char, std::string>;

class Token
{
	TokenType type;
	std::string lexeme;
	Literal lit;
	int line_num; // for error handling

	Token(TokenType type, std::string lexeme, Literal lit, int line_num) : type(type), lexeme(lexeme), lit(lit), line_num(line_num) {}

};

#endif // !TOKEN_HPP

