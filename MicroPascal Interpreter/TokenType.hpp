#ifndef TOKENTYPE_HPP
#define TOKENTYPE_HPP

#include <ostream>

enum class TokenType
{
	// single char
	SEMICOLON,
	COLON,
	DOT,
	COMMA,
	MUL,
	PLUS,
	MINUS,
	EQUAL,
	GREATER,
	LESS,
	LEFT_PAR,
	RIGHT_PAR,

	// two chars
	LESS_EQUAL,
	GREATER_EQUAL,
	NOT_EQUAL,
	ASSIGN,

	// literals
	ID,
	STRING_VAL,
	INTEGER_VAL,

	// reserved keywords
	PROGRAM,
	BEGIN,
	END,
	VAR,
	TRUE,
	FALSE,
	AND,
	OR,
	NOT,
	FOR,
	TO,
	DOWNTO,
	DO,
	WHILE,
	IF,
	THEN,
	ELSE,
	PROCEDURE,
	FUNCTION,
	WRITELN,
	STRING_TYPE,
	INTEGER_TYPE,
	BOOL_TYPE,
	DIV,

	// artificial
	END_OF_FILE
};

inline std::ostream& operator<<(std::ostream& os, const TokenType& token_type) {
	std::string type_string = "UNKNOWN";
	switch (token_type)
	{
	case TokenType::SEMICOLON:
		type_string = "SEMICOLON";
		break;
	case TokenType::COLON:
		type_string = "COLON";
		break;
	case TokenType::DOT:
		type_string = "DOT";
		break;
	case TokenType::COMMA:
		type_string = "COMMA";
		break;
	case TokenType::MUL:
		type_string = "MUL";
		break;
	case TokenType::PLUS:
		type_string = "PLUS";
		break;
	case TokenType::MINUS:
		type_string = "MINUS";
		break;
	case TokenType::DIV:
		type_string = "DIV";
		break;
	case TokenType::EQUAL:
		type_string = "EQUAL";
		break;
	case TokenType::GREATER:
		type_string = "GREATER";
		break;
	case TokenType::LESS:
		type_string = "LESS";
		break;
	case TokenType::LEFT_PAR:
		type_string = "LEFT_PAR";
		break;
	case TokenType::RIGHT_PAR:
		type_string = "RIGHT_PAR";
		break;
	case TokenType::LESS_EQUAL:
		type_string = "LESS_EQUAL";
		break;
	case TokenType::GREATER_EQUAL:
		type_string = "GREATER_EQUAL";
		break;
	case TokenType::NOT_EQUAL:
		type_string = "NOT_EQUAL";
		break;
	case TokenType::ASSIGN:
		type_string = "ASSIGN";
		break;
	case TokenType::ID:
		type_string = "ID";
		break;
	case TokenType::STRING_VAL:
		type_string = "STRING_VAL";
		break;
	case TokenType::INTEGER_VAL:
		type_string = "INTEGER_VAL";
		break;
	case TokenType::PROGRAM:
		type_string = "PROGRAM";
		break;
	case TokenType::BEGIN:
		type_string = "BEGIN";
		break;
	case TokenType::END:
		type_string = "END";
		break;
	case TokenType::VAR:
		type_string = "VAR";
		break;
	case TokenType::TRUE:
		type_string = "TRUE";
		break;
	case TokenType::FALSE:
		type_string = "FALSE";
		break;
	case TokenType::AND:
		type_string = "AND";
		break;
	case TokenType::OR:
		type_string = "OR";
		break;
	case TokenType::NOT:
		type_string = "NOT";
		break;
	case TokenType::FOR:
		type_string = "FOR";
		break;
	case TokenType::TO:
		type_string = "TO";
		break;
	case TokenType::DOWNTO:
		type_string = "DOWNTO";
		break;
	case TokenType::DO:
		type_string = "DO";
		break;
	case TokenType::WHILE:
		type_string = "WHILE";
		break;
	case TokenType::IF:
		type_string = "IF";
		break;
	case TokenType::THEN:
		type_string = "THEN";
		break;
	case TokenType::ELSE:
		type_string = "ELSE";
		break;
	case TokenType::PROCEDURE:
		type_string = "PROCEDURE";
		break;
	case TokenType::FUNCTION:
		type_string = "FUNCTION";
		break;
	case TokenType::WRITELN:
		type_string = "WRITELN";
		break;
	case TokenType::STRING_TYPE:
		type_string = "STRING_TYPE";
		break;
	case TokenType::INTEGER_TYPE:
		type_string = "INTEGER_TYPE";
		break;
	case TokenType::BOOL_TYPE:
		type_string = "BOOL_TYPE";
		break;
	case TokenType::END_OF_FILE:
		type_string = "END_OF_FILE";
		break;
	default:
		break;
	}
	return os << type_string;
}

#endif // !TOKENTYPE_HPP