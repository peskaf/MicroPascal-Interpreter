#include "Parser.hpp"
#include "Error.hpp"

Parser::Parser(std::vector<Token>& m_tokens) : tokens(m_tokens) {};

std::unique_ptr<Stmt> Parser::Parse()
{
    return Program();
}

std::unique_ptr<Stmt> Parser::Program()
{
    // header
    Eat(TokenType::PROGRAM, "'program' expected.");
    
    std::string id = Eat(TokenType::ID, "identifier expected.").lexeme;

    Eat(TokenType::SEMICOLON, "';' expected.");

    // declarations
    std::vector<std::unique_ptr<Stmt>> decl_stmts;
    while (GetCurrTok().type == TokenType::VAR || GetCurrTok().type == TokenType::PROCEDURE || GetCurrTok().type == TokenType::FUNCTION)
    {
        decl_stmts.push_back(Declaration());
    }
    
    // comp. stmt
    std::unique_ptr<Stmt> comp_stmt = CompoundStatement();
    Eat(TokenType::DOT, "'.' expected.");

    if (!IsAtEnd()) // to avoid some "code" after '.'
    {
        Error::ThrowError(GetCurrTok().line_num, "EOF expected.");
    }
    return std::make_unique<ProgramStmt>(id, std::move(comp_stmt), std::move(decl_stmts));
}

std::unique_ptr<Stmt> Parser::Statement()
{
    switch (GetCurrTok().type)
    {
    case TokenType::WRITELN:
        return WritelnStatement();
    case TokenType::BEGIN:
        return CompoundStatement();
    case TokenType::ID:
        return AssignmentStatement();
    case TokenType::IF:
        return IfStatement();
    case TokenType::WHILE:
        return WhileStatement();
    default:
        return EmptyStatement();
    }  
}

std::unique_ptr<Stmt> Parser::IfStatement()
{
    Token if_tok = Eat(TokenType::IF, "'if' expected.");
    std::unique_ptr<Expr> condition = Expression();
    Eat(TokenType::THEN, "'then' expected.");
    std::unique_ptr<Stmt> then_branch = Statement();
    if (GetCurrTok().type == TokenType::ELSE)
    {
        Eat(TokenType::ELSE, "'else' expected.");
        std::unique_ptr<Stmt> else_branch = Statement();
        return std::make_unique<IfStmt>(if_tok, std::move(condition), std::move(then_branch), std::move(else_branch));
    }
    return std::make_unique<IfStmt>(if_tok, std::move(condition), std::move(then_branch), std::nullopt);
}

std::unique_ptr<Stmt> Parser::WhileStatement()
{
    Token while_tok = Eat(TokenType::WHILE, "'while' expected.");
    std::unique_ptr<Expr> condition = Expression();
    Eat(TokenType::DO, "'do' expected.");
    std::unique_ptr<Stmt> body = Statement();

    return std::make_unique<WhileStmt>(while_tok, std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::EmptyStatement()
{
    return std::make_unique<EmptyStmt>();
}

std::unique_ptr<Stmt> Parser::AssignmentStatement()
{
    Token id = Eat(TokenType::ID, "identifier expected.");
    Eat(TokenType::ASSIGN, "':=' expected.");
    std::unique_ptr<Expr> value = Expression();

    return std::make_unique<AssignmentStmt>(id, std::move(value));
}

std::unique_ptr<Stmt> Parser::CompoundStatement()
{
    Eat(TokenType::BEGIN, "'begin' expected.");
    std::vector<std::unique_ptr<Stmt>> statement_list = StatementList();
    Eat(TokenType::END, "';' expected."); // end not found -> there should have been ';' separating statements
    return std::make_unique<CompoundStmt>(std::move(statement_list));
}

std::vector<std::unique_ptr<Stmt>> Parser::StatementList()
{
    std::vector<std::unique_ptr<Stmt>> statement_list;
    statement_list.push_back(Statement());
    while (NextMatchWith(std::vector<TokenType>{TokenType::SEMICOLON}))
    {
        statement_list.push_back(Statement());
    }
    return statement_list;
}

std::unique_ptr<Stmt> Parser::Declaration()
{
    switch(GetCurrTok().type)
    {
    case TokenType::VAR:
        return VarDecl();
    /*
    case TokenType::FUNCTION: // TODO: implement
        return ;
    case TokenType::PROCEDURE: // TODO: implement
        return ;
    */
    default:
        Error::ThrowError(GetCurrTok().line_num, "declaration expected.");
    }
}

std::unique_ptr<Stmt> Parser::VarDecl()
{
    Eat(TokenType::VAR, "'var' expected.");
    std::unordered_map<int, std::vector<Token>> variables;
    
    int type_id = 0;

    if (GetCurrTok().type != TokenType::ID)
    {
        Error::ThrowError(GetCurrTok().line_num, "identifier expected.");
    }

    while (GetCurrTok().type == TokenType::ID)
    {
        std::vector<Token> tokens; // make super tokens vector is empty
        tokens.push_back(Eat(TokenType::ID, "identifier expected."));
        while (NextMatchWith(std::vector<TokenType>{TokenType::COMMA}))
        {
            tokens.push_back(Eat(TokenType::ID, "identifier expected."));
        }
        Eat(TokenType::COLON, "':' expected.");

        switch (GetCurrTok().type)
        {
        case TokenType::INTEGER_TYPE: // 1
            type_id = 1;
            break;
        case TokenType::BOOL_TYPE: // 2
            type_id = 2;
            break;
        case TokenType::STRING_TYPE: // 3
            type_id = 3;
            break;
        default:
            Error::ThrowError(GetCurrTok().line_num, "invalid variable type.");
        }
        Advance();

        if (variables.find(type_id) != variables.end()) // type_id is already there
        {
            variables[type_id].insert(variables[type_id].end(), tokens.begin(), tokens.end()); // add variables to the same type key
        }
        else // type_id denotes a new type
        {
            variables[type_id] = tokens;
        }
        Eat(TokenType::SEMICOLON, "';' expected.");
    }
    return std::make_unique<VarDeclStmt>(variables);
}

std::unique_ptr<Stmt> Parser::WritelnStatement()
{
    Eat(TokenType::WRITELN, "'writeln' expected.");
    Eat(TokenType::LEFT_PAR, "'(' expected.");

    if (NextMatchWith(std::vector<TokenType>{TokenType::RIGHT_PAR}))
    {
        return std::make_unique<WritelnStmt>(std::nullopt);
    }

    std::vector<std::unique_ptr<Expr>> exprs;

    exprs.push_back(Expression());

    while (GetCurrTok().type != TokenType::RIGHT_PAR)
    {
        Eat(TokenType::COMMA, "',' expected,");
        exprs.push_back(Expression());
    }

    Eat(TokenType::RIGHT_PAR, "')' expected.");
    return std::make_unique<WritelnStmt>(std::move(exprs));
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

Token Parser::Eat(TokenType expected_type, std::string error_message)
{
    if (Check(expected_type))
    {
        Advance();
        return GetPrevious();
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

// Factor -> ("+" | "-" | "not") Factor | FunctionExpr | INTEGER | "true" | "false" | STRING | "(" Expression ")" | IDENTIFIER
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

    if (NextMatchWith(std::vector<TokenType>{TokenType::ID}))
    {
        return std::make_unique<VariableExpr>(std::move(GetPrevious()));
    }

    Error::ThrowError(GetCurrTok().line_num, "expression expected.");
}