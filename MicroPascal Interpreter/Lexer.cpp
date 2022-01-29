#include "Token.hpp"
#include <list>

class Lexer // lexical analysis, creates Tokens
{
public:
    Lexer(std::string m_text) : input(m_text) {};

    std::list<Token> GetTokens()
    {
        while (!isAtEnd())
        {
            start_pos = curr_pos;
            scanToken();
        }
        tokens.push_back(Token(TokenType::END_OF_FILE, "", nullptr, line_num));
        return tokens;
    }

private:
    std::string input;

    int line_num = 1;
    size_t start_pos = 0;
    size_t curr_pos = 0;
    char curr_char = input[curr_pos];

    std::list<Token> tokens;

    bool isAtEnd()
    {
        return curr_pos >= input.length();
    }

    void scanToken()
    {
        if (std::isspace(curr_char))
        {
            SkipWhitespace();
            return;
        }

        if (std::isdigit(curr_char))
        {
            addToken(TokenType::INTEGER, Integer());
            return;
        }

        switch (curr_char)
        {
        case '+':
            addToken(TokenType::PLUS);
            break;
        case '-':
            addToken(TokenType::MINUS);
            break;
        case '*':
            addToken(TokenType::MUL);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '(':
            addToken(TokenType::LEFT_PAR);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAR);
            break;
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
        case '=':
            addToken(TokenType::EQUAL);
            break;
        case '<':
            if (nextIsMatchWith('='))
            {
                addToken(TokenType::LESS_EQUAL);
            }
            else if (nextIsMatchWith('>'))
            {
                addToken(TokenType::NOT_EQUAL);
            }
            else
            {
                addToken(TokenType::LESS);
            }
            break;
        case '>':
            if (nextIsMatchWith('='))
            {
                addToken(TokenType::GREATER_EQUAL);
            }
            else
            {
                addToken(TokenType::GREATER);
            }
            break;
        case ':':
            if (nextIsMatchWith('='))
            {
                addToken(TokenType::ASSIGN);
            }
            else
            {
                addToken(TokenType::COLON);
            }
            break;
        case '{':
            while (!isAtEnd() && curr_char != '}') // skip comment
            {
                if (curr_char == '\n')
                {
                    line_num++;
                }
                Advance();
            }
            Advance(); // no effect if it's EOF, otherwise skips comment terminator
            break;
        default:
            throw std::runtime_error("Unknown char."); // later change to some error call
            break;
        }
    }

    bool nextIsMatchWith(char next)
    {
        if (isAtEnd() || input[curr_pos + 1] != next)
        {
            return false;
        }
        Advance();
        return true;
    }

    int Integer()
    {
        int number = curr_char - '0'; // make int out of a char
        while (curr_pos + 1 < input.size() && std::isdigit(input[curr_pos + 1]))
        {
            Advance();
            number = 10 * number + (curr_char - '0');
        }
        return number;
    }

    void addToken(TokenType type)
    {
        addToken(type, nullptr);
    }

    void addToken(TokenType type, Literal lit)
    {   
        Advance();
        // std::cout << start_pos << " " << curr_pos << " " << input.substr(start_pos, curr_pos - start_pos) << std::endl;
        tokens.push_back(Token(type, input.substr(start_pos, curr_pos - start_pos), lit, line_num));
    }

    void Advance()
    {
        curr_pos++;
        if (!isAtEnd())
        {
            curr_char = input[curr_pos];
        }
    }

    void SkipWhitespace()
    {
        while (!isAtEnd() && std::isspace(curr_char))
        {
            if (curr_char == '\n')
            {
                line_num++;
            }
            Advance();
        }
    }
};