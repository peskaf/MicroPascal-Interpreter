#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <string>

#include "Expr.hpp"
#include "TokenType.hpp"
#include "Token.hpp"

class Parser // syntactic analysis, creates AST, contains grammar rules
{
public:
    Parser(std::vector<Token>& m_tokens);

    std::unique_ptr<Expr> Parse();

private:
    Token GetCurrTok();

    Token GetPrevious();

    void Advance();

    bool Check(TokenType token_type);

    bool IsAtEnd();

    bool NextMatchWith(std::vector<TokenType> token_types);

    void Eat(TokenType expected_type, std::string error_message);

    std::unique_ptr<Expr> Expression();

    std::unique_ptr<Expr> SimpleExpr();

    std::unique_ptr<Expr> Term();

    std::unique_ptr<Expr> Factor();

    std::vector<Token> tokens;
    int curr_tok_num = 0;
};

#endif // !PARSER_HPP