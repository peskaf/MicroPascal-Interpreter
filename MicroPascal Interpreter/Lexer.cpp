#include <algorithm>

#include "Lexer.hpp"


Lexer::Lexer(std::string m_text) : input(m_text) {}

std::vector<Token> Lexer::GetTokens()
{
    while (!IsAtEnd())
    {
        start_pos = curr_pos;
        ScanToken();
    }
    tokens.push_back(Token(TokenType::END_OF_FILE, "", nullptr, line_num));
    return tokens;
}

bool Lexer::IsAtEnd()
{
    return curr_pos >= input.length();
}

void Lexer::ScanToken()
{
    if (std::isspace(curr_char))
    {
        SkipWhitespace();
        return;
    }

    if (std::isdigit(curr_char))
    {
        AddToken(TokenType::INTEGER_VAL, Integer());
        return;
    }

    if (std::isalpha(curr_char))
    {
        Identifier();
        return;
    }

    switch (curr_char)
    {
    case '+':
        AddToken(TokenType::PLUS);
        break;
    case '-':
        AddToken(TokenType::MINUS);
        break;
    case '*':
        AddToken(TokenType::MUL);
        break;
    case '.':
        AddToken(TokenType::DOT);
        break;
    case ',':
        AddToken(TokenType::COMMA);
        break;
    case '(':
        AddToken(TokenType::LEFT_PAR);
        break;
    case ')':
        AddToken(TokenType::RIGHT_PAR);
        break;
    case ';':
        AddToken(TokenType::SEMICOLON);
        break;
    case '=':
        AddToken(TokenType::EQUAL);
        break;
    case '<':
        if (NextIsMatchWith('='))
        {
            AddToken(TokenType::LESS_EQUAL);
        }
        else if (NextIsMatchWith('>'))
        {
            AddToken(TokenType::NOT_EQUAL);
        }
        else
        {
            AddToken(TokenType::LESS);
        }
        break;
    case '>':
        if (NextIsMatchWith('='))
        {
            AddToken(TokenType::GREATER_EQUAL);
        }
        else
        {
            AddToken(TokenType::GREATER);
        }
        break;
    case ':':
        if (NextIsMatchWith('='))
        {
            AddToken(TokenType::ASSIGN);
        }
        else
        {
            AddToken(TokenType::COLON);
        }
        break;
    case '\'': // '
        AddToken(TokenType::STRING_VAL, String());
        break;
    case '{':
        SkipComment();
        break;
    default:
        throw std::runtime_error("Unknown char."); // later change to some error call mby
        break;
    }
}

void Lexer::SkipComment()
{
    Advance(); // go past the first '{'

    int braces_count = 1; // +1 if see {, -1 if see } -> allows nested comments
    while (!IsAtEnd() && braces_count != 0) // skip comment
    {
        if (curr_char == '\n')
        {
            line_num++;
        }
        else if (curr_char == '{')
        {
            braces_count++;
        }
        else if (curr_char == '}')
        {
            braces_count--;
        }

        Advance();
    }

    if (IsAtEnd() && braces_count != 0)
    {
        throw std::runtime_error("Unexpected end of file."); // comment is not terminated -> goes on until the EOF
    }
}

bool Lexer::NextIsMatchWith(char next)
{
    if (IsAtEnd() || input[curr_pos + 1] != next)
    {
        return false;
    }
    Advance();
    return true;
}

int Lexer::Integer()
{
    int number = curr_char - '0'; // make int out of a char
    while (curr_pos + 1 < input.size() && std::isdigit(input[curr_pos + 1]))
    {
        Advance();
        number = 10 * number + (curr_char - '0');
    }
    return number;
}

void Lexer::Identifier()
{
    while (curr_pos + 1 < input.size() && std::isalnum(input[curr_pos + 1]))
    {
        Advance();
    }

    std::string lit_value = input.substr(start_pos, curr_pos - start_pos + 1); // + 1 -> need lit_value, in AddToken it will be advanced
    std::transform(lit_value.begin(), lit_value.end(), lit_value.begin(), // Pascal is case insensitive
        [](unsigned char c) { return std::tolower(c); });

    if (reserved_keywords.find(lit_value) != reserved_keywords.end())
    {
        if (lit_value == "true")
        {
            AddToken(TokenType::TRUE, true);
            return;
        }
            
        if (lit_value == "false")
        {
            AddToken(TokenType::FALSE, false);
            return;
        }

        AddToken(reserved_keywords.at(lit_value));
        return;
    }
    AddToken(TokenType::ID);
}

std::string Lexer::String()
{
    while (curr_pos + 1 < input.size() && input[curr_pos + 1] != '\'')
    {
        Advance();
        if (curr_char == '\n')
        {
            throw std::runtime_error("String exceeds line."); // later to some error class call
        }
    }
    if (IsAtEnd())
    {
        throw std::runtime_error("String not terminated."); // later to some error class call
    }
    Advance(); // skip second '
    return input.substr(start_pos + 1, curr_pos - (start_pos + 1)); // cut ' chars
}

void Lexer::AddToken(TokenType type)
{
    AddToken(type, nullptr);
}

void Lexer::AddToken(TokenType type, Literal lit)
{   
    Advance();
    std::string lexeme = input.substr(start_pos, curr_pos - start_pos);
    if (type == TokenType::ID)
    {
        // Pascal is case insensitive -> ID's lexeme gets converted to lower so we dont have to do so later (for looking up value etc.)
        std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(),
            [](unsigned char c) { return std::tolower(c); });
    }
        
    tokens.push_back(Token(type, lexeme, lit, line_num));
}

void Lexer::Advance()
{
    curr_pos++;
    if (!IsAtEnd())
    {
        curr_char = input[curr_pos];
    }
}

void Lexer::SkipWhitespace()
{
    while (!IsAtEnd() && std::isspace(curr_char))
    {
        if (curr_char == '\n')
        {
            line_num++;
        }
        Advance();
    }
}
