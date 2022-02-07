#include <algorithm>

#include "Lexer.hpp"
#include "Error.hpp"

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

    if (std::isalpha(curr_char) || curr_char == '_')
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
        throw Error(line_num, "illegal character.");
        break;
    }
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
        // Pascal is case insensitive -> ID's lexeme gets converted to lower so it must not be done later (for looking up value etc.)
        std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), [](unsigned char c) { return std::tolower(c); }); // transform to lower
    }

    tokens.push_back(Token(type, lexeme, lit, line_num));
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
    // advance while it's part of ID name
    while (curr_pos + 1 < input.size() && (std::isalnum(input[curr_pos + 1]) || input[curr_pos + 1] == '_'))
    {
        Advance();
    }

    // get lexeme and transform to lower
    std::string lit_value = input.substr(start_pos, curr_pos - start_pos + 1); // + 1 -> need lit_value, in AddToken it will be advanced
    std::transform(lit_value.begin(), lit_value.end(), lit_value.begin(), [](unsigned char c) { return std::tolower(c); }); // transform to lower

    // check for reserved keyword
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
    // advance while it's part of the string
    while (curr_pos + 1 < input.size() && input[curr_pos + 1] != '\'')
    {
        Advance();

        // string cannot exceed line
        if (curr_char == '\n')
        {
            throw Error(line_num, "string exceeds line.");
        }
    }

    // string has to be terminated by '
    if (IsAtEnd())
    {
        throw Error(line_num, "string is not terminated.");
    }

    Advance(); // skip second '
    return input.substr(start_pos + 1, curr_pos - (start_pos + 1)); // +1's -> cut ' chars
}

// assign current char if not the end
void Lexer::Advance()
{
    curr_pos++;
    if (!IsAtEnd())
    {
        curr_char = input[curr_pos];
    }
}

// check if next char matches with given one, advances if there is match
bool Lexer::NextIsMatchWith(char next)
{
    if (IsAtEnd() || input[curr_pos + 1] != next)
    {
        return false;
    }

    // match
    Advance();
    return true;
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

void Lexer::SkipComment()
{
    Advance(); // go past the first '{'

    int braces_count = 1; // +1 if see '{', -1 if see '}' -> allows nested comments
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

    // comment must be terminated by '{'
    if (IsAtEnd() && braces_count != 0)
    {
        throw Error(line_num, "unexpected EOF."); // comment goes on until the EOF
    }
}

bool Lexer::IsAtEnd()
{
    return curr_pos >= input.length();
}