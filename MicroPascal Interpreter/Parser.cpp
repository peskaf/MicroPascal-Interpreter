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
    std::vector<std::unique_ptr<Stmt>> decl_stmts = Declarations();
    
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
        if (Peek().type == TokenType::ASSIGN)
        {
            return AssignmentStatement();
        }
        return ProcStmt();
    case TokenType::IF:
        return IfStatement();
    case TokenType::WHILE:
        return WhileStatement();
    case TokenType::FOR:
        return ForStatement();
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

std::unique_ptr<Stmt> Parser::ForStatement()
{
    Token for_tok = Eat(TokenType::FOR, "'for' expected.");
    if (GetCurrTok().type != TokenType::ID)
    {
        Error::ThrowError(GetCurrTok().line_num, "identifier expected.");
    }

    Token id_tok = GetCurrTok();
    std::unique_ptr<Stmt> assignment = AssignmentStatement();

    bool increment;
    if (GetCurrTok().type == TokenType::TO)
    {
        increment = true;
    }
    else if (GetCurrTok().type == TokenType::DOWNTO)
    {
        increment = false;
    }
    else
    {
        Error::ThrowError(GetCurrTok().line_num, "'to' or 'downto' expected.");
    }
    Advance();

    std::unique_ptr<Expr> expression = Expression();
    Eat(TokenType::DO, "'do' expected.");
    std::unique_ptr<Stmt> body = Statement();

    return std::make_unique<ForStmt>(for_tok, increment, id_tok, std::move(assignment), std::move(expression), std::move(body));
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
    case TokenType::FUNCTION:
        return FuncDecl();
    case TokenType::PROCEDURE:
        return ProcDecl();
    default:
        Error::ThrowError(GetCurrTok().line_num, "declaration expected.");
    }
}

std::vector<Token> Parser::IdentifierList()
{
    std::vector<Token> tokens;
    tokens.push_back(Eat(TokenType::ID, "identifier expected."));
    while (NextMatchWith(std::vector<TokenType>{TokenType::COMMA}))
    {
        tokens.push_back(Eat(TokenType::ID, "identifier expected."));
    }
    return tokens;
}

std::unique_ptr<Stmt> Parser::VarDecl()
{
    Eat(TokenType::VAR, "'var' expected.");
    std::unordered_map<VariableType, std::vector<Token>> variables;
    
    VariableType type;

    if (GetCurrTok().type != TokenType::ID)
    {
        Error::ThrowError(GetCurrTok().line_num, "identifier expected.");
    }

    while (GetCurrTok().type == TokenType::ID)
    {
        std::vector<Token> tokens = IdentifierList();

        Eat(TokenType::COLON, "':' expected.");
        switch (GetCurrTok().type)
        {
        case TokenType::INTEGER_TYPE:
            type = VariableType::INTEGER;
            break;
        case TokenType::BOOL_TYPE:
            type = VariableType::BOOL;
            break;
        case TokenType::STRING_TYPE:
            type = VariableType::STRING;
            break;
        default:
            Error::ThrowError(GetCurrTok().line_num, "invalid variable type.");
        }
        Advance();

        if (variables.find(type) != variables.end()) // type_id is already there
        {
            variables[type].insert(variables[type].end(), tokens.begin(), tokens.end()); // add variables to the same type key
        }
        else // type_id denotes a new type
        {
            variables[type] = tokens;
        }
        Eat(TokenType::SEMICOLON, "';' expected.");
    }
    return std::make_unique<VarDeclStmt>(variables);
}

std::unique_ptr<Stmt> Parser::FuncDecl()
{
    Eat(TokenType::FUNCTION, "'function' expected.");
    Token id_token = Eat(TokenType::ID, "identifier expected.");
    std::vector<std::pair<Token, VariableType>> parameter_list{};

    // is there parameter list?
    if (GetCurrTok().type == TokenType::LEFT_PAR)
    {
        parameter_list = ParameterList();
    }

    Eat(TokenType::COLON, "':' expected.");

    // return type
    VariableType return_type;
    switch (GetCurrTok().type)
    {
    case TokenType::INTEGER_TYPE:
        return_type = VariableType::INTEGER;
        break;
    case TokenType::BOOL_TYPE:
        return_type = VariableType::BOOL;
        break;
    case TokenType::STRING_TYPE:
        return_type = VariableType::STRING;
        break;
    default:
        Error::ThrowError(GetCurrTok().line_num, "type expected.");
        break;
    }
    Advance();

    Eat(TokenType::SEMICOLON, "';' expected.");

    // declarations
    std::vector<std::unique_ptr<Stmt>> decl_stmts = Declarations();

    // body
    std::unique_ptr<Stmt> body = CompoundStatement();

    Eat(TokenType::SEMICOLON, "';' expected.");

    return std::make_unique<FuncDeclStmt>(id_token, return_type, std::move(body), std::move(decl_stmts), std::move(parameter_list));
}

std::unique_ptr<Stmt> Parser::ProcDecl()
{
    Eat(TokenType::PROCEDURE, "'procedure' expected.");
    Token id_token = Eat(TokenType::ID, "identifier expected.");
    std::vector<std::pair<Token, VariableType>> parameter_list{};

    // is there parameter list?
    if (GetCurrTok().type == TokenType::LEFT_PAR)
    {
        parameter_list = ParameterList();
    }

    Eat(TokenType::SEMICOLON, "';' expected.");

    // declarations
    std::vector<std::unique_ptr<Stmt>> decl_stmts = Declarations();

    // body
    std::unique_ptr<Stmt> body = CompoundStatement();

    Eat(TokenType::SEMICOLON, "';' expected.");

    return std::make_unique<ProcDeclStmt>(id_token, std::move(body), std::move(decl_stmts), std::move(parameter_list));
}

std::unique_ptr<Stmt> Parser::ProcStmt()
{
    Token id_token = Eat(TokenType::ID, "identifier expected.");
    std::vector<std::unique_ptr<Expr>> exprs{};

    // call with arg list
    if (GetCurrTok().type == TokenType::LEFT_PAR)
    {
        Eat(TokenType::LEFT_PAR, "'(' expected.");

        // no expr list (i.e. empty)
        if (NextMatchWith(std::vector<TokenType>{TokenType::RIGHT_PAR}))
        {
            return std::make_unique<ProcedureCallStmt>(std::move(exprs), id_token);
        }

        exprs = ExprList();

        Eat(TokenType::RIGHT_PAR, "')' expected.");
    }

    return std::make_unique<ProcedureCallStmt>(std::move(exprs), id_token);
}

std::vector<std::unique_ptr<Stmt>> Parser::Declarations()
{
    std::vector<std::unique_ptr<Stmt>> decl_stmts{};
    while (GetCurrTok().type == TokenType::VAR || GetCurrTok().type == TokenType::PROCEDURE || GetCurrTok().type == TokenType::FUNCTION)
    {
        decl_stmts.push_back(Declaration());
    }
    return decl_stmts;
}

std::vector<std::pair<Token, VariableType>> Parser::ParameterList()
{
    Eat(TokenType::LEFT_PAR, "'(' expected.");
    
    // if there is no identifier -> there must be right par.
    if (GetCurrTok().type != TokenType::ID)
    {
        Eat(TokenType::RIGHT_PAR, "')' expected.");
        return {}; //return empty vector
    }

    std::vector<Token> identifiers;
    std::vector<std::pair<Token, VariableType>> parameter_list{};

    do
    {
        // get ids
        identifiers = IdentifierList();
        Eat(TokenType::COLON, "':' expected.");

        // get type
        VariableType type;
        switch (GetCurrTok().type)
        {
        case TokenType::INTEGER_TYPE:
            type = VariableType::INTEGER;
            break;
        case TokenType::BOOL_TYPE:
            type = VariableType::BOOL;
            break;
        case TokenType::STRING_TYPE:
            type = VariableType::STRING;
            break;
        default:
            Error::ThrowError(GetCurrTok().line_num, "type expected.");
            break;
        }
        Advance();

        // put all into parameter list
        for (auto&& identifier : identifiers)
        {
            parameter_list.push_back(std::make_pair(identifier, type));
        }

    } while (NextMatchWith(std::vector<TokenType>{TokenType::SEMICOLON}));

    Eat(TokenType::RIGHT_PAR, "')' expected."); 
    return parameter_list;
}

std::unique_ptr<Stmt> Parser::WritelnStatement()
{
    Eat(TokenType::WRITELN, "'writeln' expected.");
    Eat(TokenType::LEFT_PAR, "'(' expected.");

    std::vector<std::unique_ptr<Expr>> exprs{};

    // no expr list (i.e. empty)
    if (NextMatchWith(std::vector<TokenType>{TokenType::RIGHT_PAR}))
    {
        return std::make_unique<WritelnStmt>(std::move(exprs));
    }

    exprs = ExprList();

    Eat(TokenType::RIGHT_PAR, "')' expected.");
    return std::make_unique<WritelnStmt>(std::move(exprs));
}

std::vector<std::unique_ptr<Expr>> Parser::ExprList()
{
    std::vector<std::unique_ptr<Expr>> exprs;

    exprs.push_back(Expression());

    while (GetCurrTok().type == TokenType::COMMA)
    {
        Eat(TokenType::COMMA, "',' expected,");
        exprs.push_back(Expression());
    }
    return exprs;
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

std::unique_ptr<Expr> Parser::FuncExpr()
{
    Token id_token = Eat(TokenType::ID, "identifier expected.");
    std::vector<std::unique_ptr<Expr>> exprs{};

    Eat(TokenType::LEFT_PAR, "'(' expected.");

    // no expr list (i.e. empty)
    if (NextMatchWith(std::vector<TokenType>{TokenType::RIGHT_PAR}))
    {
        return std::make_unique<FunctionCallExpr>(std::move(exprs), id_token);
    }

    exprs = ExprList();

    Eat(TokenType::RIGHT_PAR, "')' expected.");

    return std::make_unique<FunctionCallExpr>(std::move(exprs), id_token);
}


// Factor -> ("+" | "-" | "not") Factor | FuncExpr | INTEGER | "true" | "false" | STRING | "(" Expression ")" | IDENTIFIER
std::unique_ptr<Expr> Parser::Factor()
{
    // TODO: predelat CELE na nejake smysluplne metody pro check jestli currtok ma nejaky typ nebo nejaky z typu
    if (GetCurrTok().type == TokenType::PLUS || GetCurrTok().type == TokenType::MINUS || GetCurrTok().type == TokenType::NOT)
    {
        Token op = Eat(GetCurrTok().type, "unary operator expected."); // will not throw
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

    if (GetCurrTok().type == TokenType::ID)
    {
        if (Peek().type == TokenType::LEFT_PAR) // function call
        {
            return FuncExpr();
        }

        return std::make_unique<VariableExpr>(Eat(TokenType::ID, "identifier expected."));
    }

    Error::ThrowError(GetCurrTok().line_num, "expression expected.");
}

Token Parser::Peek()
{
    if (!IsAtEnd())
    {
        return tokens[curr_tok_num + 1];
    }
    return GetCurrTok();
}