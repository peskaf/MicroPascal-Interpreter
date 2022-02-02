#include <vector>

#include "Expr.cpp" // rozdelit expr do hpp a cpp !!!!




class Parser // syntactic analysis, creates AST, contains grammar rules
{
public:
    Parser(std::vector<Token>& m_tokens) : tokens(m_tokens) {};

    std::unique_ptr<Expr> Parse()
    {
        return Expression();
    }

private:

    Token GetCurrTok()
    {
        return tokens[curr_tok_num];
    }

    Token GetPrevious()
    {
        return tokens[curr_tok_num - 1];
    }

    void Advance()
    {
        if (!IsAtEnd())
        {
            curr_tok_num++;
        }
    }

    bool Check(TokenType token_type)
    {
        if (IsAtEnd())
        {
            return false;
        }
        return GetCurrTok().type == token_type;
    }

    bool IsAtEnd()
    {
        return GetCurrTok().type == TokenType::END_OF_FILE;
    }

    bool NextMatchWith(std::vector<TokenType> token_types)
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

    // Expr -> SimpleExpr ((">=" | "<=" | "<>" | "=" | ">" | "<") SimpleExpr)?
    std::unique_ptr<Expr> Expression()
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
    std::unique_ptr<Expr> SimpleExpr()
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
    std::unique_ptr<Expr> Term()
    {
        const std::vector<TokenType> operators
        {
            TokenType::MUL,
            TokenType::DIV,
            TokenType::AND
        };

        std::unique_ptr<Expr> factor = Term();

        while (NextMatchWith(operators))
        {
            Token op = GetPrevious();
            std::unique_ptr<Expr> right = Term();
            factor = std::make_unique<BinaryExpr>(std::move(factor), std::move(right), op);
        }
        return factor;
    }

    // Factor -> ("+" | "-" | "not") Factor | FunctionStmt (TODO) | INTEGER | "(" Expression ")" | "true" | "false" | STRING | CHAR -- (function stmt je tedy expr? -> to vyresit)
    std::unique_ptr<Expr> Factor()
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
            if (NextMatchWith(std::vector<TokenType>{TokenType::RIGHT_PAR}))
            {
                return std::make_unique<GroupingExpr>(std::move(expr));
            }
            else
            {
                throw std::runtime_error("Expected ')' after expression.");
            }
        }
    }

    std::vector<Token> tokens;
    int curr_tok_num = 0;
};