#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <map>
#include <vector>

#include "TokenType.hpp"
#include "Token.hpp"

class Lexer // lexical analysis, creates Tokens
{
public:
    Lexer(std::string m_text);

    std::vector<Token> GetTokens();

private:
    bool IsAtEnd();

    void ScanToken();

    void SkipComment();
    
    bool NextIsMatchWith(char next);

    int Integer();

    void Identifier();

    std::string String();

    void AddToken(TokenType type);

    void AddToken(TokenType type, Literal lit);

    void Advance();

    void SkipWhitespace();

    std::string input;

    const std::map<std::string, TokenType> reserved_keywords =
    {
        {"program", TokenType::PROGRAM},
        {"begin", TokenType::BEGIN},
        {"end", TokenType::END},
        {"var", TokenType::VAR},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"and", TokenType::AND},
        {"or", TokenType::OR},
        {"not", TokenType::NOT},
        {"for", TokenType::FOR},
        {"to", TokenType::TO},
        {"downto", TokenType::DOWNTO},
        {"do", TokenType::DO},
        {"while", TokenType::WHILE},
        {"if", TokenType::IF},
        {"then", TokenType::THEN},
        {"else", TokenType::ELSE},
        {"procedure", TokenType::PROCEDURE},
        {"function", TokenType::FUNCTION},
        {"writeln", TokenType::WRITELN},
        {"string", TokenType::STRING_TYPE},
        {"char", TokenType::CHAR_TYPE},
        {"integer", TokenType::INTEGER_TYPE},
        {"boolean", TokenType::BOOL_TYPE},
        {"div", TokenType::DIV}
    };

    std::vector<Token> tokens;

    int line_num = 1;
    size_t start_pos = 0;
    size_t curr_pos = 0;
    char curr_char = input[curr_pos];
};

#endif // !LEXER_HPP
