#include "Parser.hpp"
#include "Error.hpp"

Parser::Parser(std::vector<Token>& m_tokens) : tokens(m_tokens) {};

std::vector<std::unique_ptr<Stmt>> Parser::Parse()
{
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!IsAtEnd())
    {
        statements.push_back(Statement());
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::Statement()
{
    if (NextMatchWith(std::vector<TokenType>{TokenType::WRITELN}))
    {
        return WritelnStatement();
    }
    Error::ThrowError(GetCurrTok().line_num, "statement expected.");
}

std::unique_ptr<Stmt> Parser::WritelnStatement()
{
    Eat(TokenType::LEFT_PAR, "'(' expected.");
    std::unique_ptr<Expr> to_print = Expression();
    Eat(TokenType::RIGHT_PAR, "')' expected.");
    return std::make_unique<WritelnStmt>(std::move(to_print));
}


Token Parser::GetCurrTok()
{
    return tokens[curr_tok_num];
}

Token Parser::GetPrevious()
{
    return tokens[curr_tok_num - 1];
}

void Parser::Advance()
{
    if (!IsAtEnd())
    {
        curr_tok_num++;
    }
}

bool Parser::Check(TokenType token_type)
{
    if (IsAtEnd())
    {
        return false;
    }
    return GetCurrTok().type == token_type;
}

bool Parser::IsAtEnd()
{
    return GetCurrTok().type == TokenType::END_OF_FILE;
}

bool Parser::NextMatchWith(std::vector<TokenType> token_types)
{
    for (auto&& token_type : token_types)
    {
        if (GetCurrTok().type == token_type)
        {
            Advance();
            return true;
        }
    }
    return false;
}

void Parser::Eat(TokenType expected_type, std::string error_message)
{
    if (Check(expected_type))
    {
        Advance();
        return;
    }
    
    Error::ThrowError(GetCurrTok().line_num, error_message);
}

// Expr -> SimpleExpr ((">=" | "<=" | "<>" | "=" | ">" | "<") SimpleExpr)?
std::unique_ptr<Expr> Parser::Expression()
{
    const std::vector<TokenType> operators
    {
        TokenType::GREATER_EQUAL,
        TokenType::LESS_EQUAL,
        TokenType::GREATER,
        TokenType::LESS,
        TokenType::EQUAL,
        TokenType::NOT_EQUAL
    };

    std::unique_ptr<Expr> expr = SimpleExpr();
    if (NextMatchWith(operators))
    {
        Token op = GetPrevious();
        std::unique_ptr<Expr> right = SimpleExpr();
        return std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}

// SimpleExpr -> Term (("+" | "-" | "or") Term)*
std::unique_ptr<Expr> Parser::SimpleExpr()
{
    const std::vector<TokenType> operators
    {
        TokenType::PLUS,
        TokenType::MINUS,
        TokenType::OR
    };

    std::unique_ptr<Expr> term = Term();

    while (NextMatchWith(operators))
    {
        Token op = GetPrevious();
        std::unique_ptr<Expr> right = Term();
        term = std::make_unique<BinaryExpr>(std::move(term), std::move(right), op);
    }
    return term;
}

// Term -> Factor (("*" | "div" | "and") Factor)*;
std::unique_ptr<Expr> Parser::Term()
{
    const std::vector<TokenType> operators
    {
        TokenType::MUL,
        TokenType::DIV,
        TokenType::AND
    };

    std::unique_ptr<Expr> factor = Factor();

    while (NextMatchWith(operators))
    {
        Token op = GetPrevious();
        std::unique_ptr<Expr> right = Factor();
        factor = std::make_unique<BinaryExpr>(std::move(factor), std::move(right), op);
    }
    return factor;
}

// Factor -> ("+" | "-" | "not") Factor | FunctionExpr | INTEGER | "true" | "false" | STRING | CHAR | "(" Expression ")"
std::unique_ptr<Expr> Parser::Factor()
{
    const std::vector<TokenType> unary_operators
    {
        TokenType::PLUS,
        TokenType::MINUS,
        TokenType::NOT
    };

    if (NextMatchWith(unary_operators))
    {
        Token op = GetPrevious();
        std::unique_ptr<Expr> fact = Factor();
        return std::make_unique<UnaryExpr>(std::move(fact), op);
    }

    if (NextMatchWith(std::vector<TokenType>{TokenType::INTEGER_VAL, TokenType::TRUE, TokenType::FALSE, TokenType::STRING_VAL}))
    {
        return std::make_unique<LiteralExpr>(std::move(GetPrevious().lit));
    }

    if (NextMatchWith(std::vector<TokenType>{TokenType::LEFT_PAR}))
    {
        std::unique_ptr<Expr> expr = Expression();
        Eat(TokenType::RIGHT_PAR, "')' expected after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }

    Error::ThrowError(GetCurrTok().line_num, "expression expected.");
}