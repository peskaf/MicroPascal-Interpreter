#include "Parser.hpp"
#include "Error.hpp"

Parser::Parser(std::vector<Token>& m_tokens) : tokens(m_tokens) {};

std::unique_ptr<Stmt> Parser::Parse()
{
    return Program();
}


// program -> "program" IDENTIFIER ";" declaration* compoundStmt "." EOF;
std::unique_ptr<Stmt> Parser::Program()
{
    // header
    Eat(TokenType::PROGRAM, "'program' expected.");
    
    std::string id = Eat(TokenType::ID, "identifier expected.").lexeme;

    Eat(TokenType::SEMICOLON, "';' expected.");

    // declarations
    std::vector<std::shared_ptr<Stmt>> decl_stmts = Declarations();
    
    // comp. stmt
    std::unique_ptr<Stmt> comp_stmt = CompoundStatement();
    Eat(TokenType::DOT, "'.' expected.");

    if (!IsAtEnd()) // to avoid some "code" after '.'
    {
        throw Error(GetCurrTok().line_num, "EOF expected.");
    }
    return std::make_unique<ProgramStmt>(id, std::move(comp_stmt), std::move(decl_stmts));
}


// declaration*
std::vector<std::shared_ptr<Stmt>> Parser::Declarations()
{
    std::vector<std::shared_ptr<Stmt>> decl_stmts{};
    while (CurrTokIs(TokenType::VAR) || CurrTokIs(TokenType::PROCEDURE) || CurrTokIs(TokenType::FUNCTION))
    {
        decl_stmts.push_back(Declaration());
    }
    return decl_stmts;
}

// declaration -> procDecl | funcDecl | varDecl;
std::shared_ptr<Stmt> Parser::Declaration()
{
    switch (GetCurrTok().type)
    {
    case TokenType::PROCEDURE:
        return ProcDecl();
    case TokenType::FUNCTION:
        return FuncDecl();
    case TokenType::VAR:
        return VarDecl();
    default:
        throw Error(GetCurrTok().line_num, "declaration expected.");
    }
}

// procDecl -> "procedure" IDENTIFIER parameterList? ";" declaration* compoundStmt ";";
std::shared_ptr<Stmt> Parser::ProcDecl()
{
    Eat(TokenType::PROCEDURE, "'procedure' expected.");
    Token id_token = Eat(TokenType::ID, "identifier expected.");
    std::vector<std::pair<Token, VariableType>> parameter_list{};

    // check for parameter list
    if (GetCurrTok().type == TokenType::LEFT_PAR)
    {
        parameter_list = ParameterList();
    }

    Eat(TokenType::SEMICOLON, "';' expected.");

    // declarations
    std::vector<std::shared_ptr<Stmt>> decl_stmts = Declarations();

    // body
    std::unique_ptr<Stmt> body = CompoundStatement();

    Eat(TokenType::SEMICOLON, "';' expected.");

    return std::make_shared<ProcDeclStmt>(id_token, std::move(body), std::move(decl_stmts), std::move(parameter_list));
}

// funcDecl -> "function" IDENTIFIER parameterList? ":" type ";" declaration* compoundStmt ";";
std::shared_ptr<Stmt> Parser::FuncDecl()
{
    Eat(TokenType::FUNCTION, "'function' expected.");
    Token id_token = Eat(TokenType::ID, "identifier expected.");

    std::vector<std::pair<Token, VariableType>> parameter_list{};

    // check for parameter list
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
        throw Error(GetCurrTok().line_num, "type expected.");
    }
    Advance();

    Eat(TokenType::SEMICOLON, "';' expected.");

    // declarations
    std::vector<std::shared_ptr<Stmt>> decl_stmts = Declarations();

    // body
    std::unique_ptr<Stmt> body = CompoundStatement();

    Eat(TokenType::SEMICOLON, "';' expected.");

    return std::make_shared<FuncDeclStmt>(id_token, return_type, std::move(body), std::move(decl_stmts), std::move(parameter_list));
}

// varDecl -> "var" (identifierList ":" type ";")+ ;
std::shared_ptr<Stmt> Parser::VarDecl()
{
    Eat(TokenType::VAR, "'var' expected.");
    std::unordered_map<VariableType, std::vector<Token>> variables;


    // no identifier found
    if (!CurrTokIs(TokenType::ID))
    {
        throw Error(GetCurrTok().line_num, "identifier expected.");
    }

    VariableType type;
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
            throw Error(GetCurrTok().line_num, "invalid variable type.");
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
    return std::make_shared<VarDeclStmt>(variables);
}


// statement -> writelnStmt | procedureStmt | compoundStmt | ifStmt | forStmt | whileStmt | assignStmt | emptyStmt;
std::unique_ptr<Stmt> Parser::Statement()
{
    switch (GetCurrTok().type)
    {
    case TokenType::WRITELN:
        return WritelnStatement();
    case TokenType::BEGIN:
        return CompoundStatement();
    case TokenType::ID:
        // check for ':=' after ID
        if (NextTokIs(TokenType::ASSIGN))
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

// writelnStmt -> "writeln" "(" exprList? ")";
std::unique_ptr<Stmt> Parser::WritelnStatement()
{
    Eat(TokenType::WRITELN, "'writeln' expected.");
    Eat(TokenType::LEFT_PAR, "'(' expected.");

    std::vector<std::unique_ptr<Expr>> exprs{};

    // no expr list (i.e. empty)
    if (CurrTokIs(TokenType::RIGHT_PAR))
    {
        Advance(); // skip the ')'
        return std::make_unique<WritelnStmt>(std::move(exprs));
    }

    exprs = ExprList();

    Eat(TokenType::RIGHT_PAR, "')' expected.");
    return std::make_unique<WritelnStmt>(std::move(exprs));
}

// procedureStmt -> IDENTIFIER ("(" exprList ")")?;
std::unique_ptr<Stmt> Parser::ProcStmt()
{
    Token id_token = Eat(TokenType::ID, "identifier expected.");
    std::vector<std::unique_ptr<Expr>> exprs{};

    // call with arg list
    if (CurrTokIs(TokenType::LEFT_PAR))
    {
        Advance(); // skip the '('

        // no expr list (i.e. empty)
        if (CurrTokIs(TokenType::RIGHT_PAR))
        {
            Advance(); // skip the ')'
            return std::make_unique<ProcedureCallStmt>(std::move(exprs), id_token);
        }

        exprs = ExprList();

        Eat(TokenType::RIGHT_PAR, "')' expected.");
    }

    return std::make_unique<ProcedureCallStmt>(std::move(exprs), id_token);
}

// compoundStmt -> "begin" statementList "end";
std::unique_ptr<Stmt> Parser::CompoundStatement()
{
    Eat(TokenType::BEGIN, "'begin' expected.");
    std::vector<std::unique_ptr<Stmt>> statement_list = StatementList();
    Eat(TokenType::END, "';' expected."); // end not found -> there should have been ';' separating statements

    return std::make_unique<CompoundStmt>(std::move(statement_list));
}

// ifStmt -> "if" expression "then" statement ("else" statement)?;
std::unique_ptr<Stmt> Parser::IfStatement()
{
    Token if_tok = Eat(TokenType::IF, "'if' expected.");
    std::unique_ptr<Expr> condition = Expression();

    Eat(TokenType::THEN, "'then' expected.");
    std::unique_ptr<Stmt> then_branch = Statement();

    if (CurrTokIs(TokenType::ELSE))
    {
        Eat(TokenType::ELSE, "'else' expected.");
        std::unique_ptr<Stmt> else_branch = Statement();

        return std::make_unique<IfStmt>(if_tok, std::move(condition), std::move(then_branch), std::move(else_branch));
    }
    return std::make_unique<IfStmt>(if_tok, std::move(condition), std::move(then_branch), std::nullopt);
}

// forStmt -> "for" assignStmt("to" | "downto") expression "do" statement;
std::unique_ptr<Stmt> Parser::ForStatement()
{
    Token for_tok = Eat(TokenType::FOR, "'for' expected.");
    if (!CurrTokIs(TokenType::ID))
    {
        throw Error(GetCurrTok().line_num, "identifier expected.");
    }

    Token it_variable_tok = GetCurrTok(); // id of iterator variable

    std::unique_ptr<Stmt> assignment = AssignmentStatement();

    // increment or decrement check
    bool increment;
    if (CurrTokIs(TokenType::TO))
    {
        increment = true;
    }
    else if (CurrTokIs(TokenType::DOWNTO))
    {
        increment = false;
    }
    else
    {
        throw Error(GetCurrTok().line_num, "'to' or 'downto' expected.");
    }
    Advance();

    std::unique_ptr<Expr> expression = Expression();

    Eat(TokenType::DO, "'do' expected.");
    std::unique_ptr<Stmt> body = Statement();

    return std::make_unique<ForStmt>(for_tok, increment, it_variable_tok, std::move(assignment), std::move(expression), std::move(body));
}

// whileStmt -> "while" expression "do" statement;
std::unique_ptr<Stmt> Parser::WhileStatement()
{
    Token while_tok = Eat(TokenType::WHILE, "'while' expected.");
    std::unique_ptr<Expr> condition = Expression();

    Eat(TokenType::DO, "'do' expected.");
    std::unique_ptr<Stmt> body = Statement();

    return std::make_unique<WhileStmt>(while_tok, std::move(condition), std::move(body));
}

// assignStmt -> IDENTIFIER ":=" expression;
std::unique_ptr<Stmt> Parser::AssignmentStatement()
{
    Token id = Eat(TokenType::ID, "identifier expected.");
    Eat(TokenType::ASSIGN, "':=' expected.");
    std::unique_ptr<Expr> value = Expression();

    return std::make_unique<AssignmentStmt>(id, std::move(value));
}

// emptyStmt -> ;
std::unique_ptr<Stmt> Parser::EmptyStatement()
{
    return std::make_unique<EmptyStmt>();
}


// expression -> simpleExpr ((">=" | "<=" | "<>" | "=" | ">" | "<") simpleExpr)?;
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
    if (CurrMatchWith(operators))
    {
        Token op = GetPrevTok();
        std::unique_ptr<Expr> right = SimpleExpr();
        return std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }
    return expr;
}

// simpleExpr -> term (("+" | "-" | "or") term)*;
std::unique_ptr<Expr> Parser::SimpleExpr()
{
    const std::vector<TokenType> operators
    {
        TokenType::PLUS,
        TokenType::MINUS,
        TokenType::OR
    };

    std::unique_ptr<Expr> term = Term();

    while (CurrMatchWith(operators))
    {
        Token op = GetPrevTok();
        std::unique_ptr<Expr> right = Term();
        term = std::make_unique<BinaryExpr>(std::move(term), std::move(right), op);
    }
    return term;
}

// term -> factor (("*" | "div" | "and") factor)*;
std::unique_ptr<Expr> Parser::Term()
{
    const std::vector<TokenType> operators
    {
        TokenType::MUL,
        TokenType::DIV,
        TokenType::AND
    };

    std::unique_ptr<Expr> factor = Factor();

    while (CurrMatchWith(operators))
    {
        Token op = GetPrevTok();
        std::unique_ptr<Expr> right = Factor();
        factor = std::make_unique<BinaryExpr>(std::move(factor), std::move(right), op);
    }
    return factor;
}

// factor -> ("+" | "-" | "not") factor | INTEGER | STRING | "true" | "false" | "(" expression ")" | IDENTIFIER | functionExpr;
std::unique_ptr<Expr> Parser::Factor()
{
    // ("+" | "-" | "not") factor
    if (CurrTokIs(TokenType::PLUS) || CurrTokIs(TokenType::MINUS) || CurrTokIs(TokenType::NOT))
    {
        Token op = Eat(GetCurrTok().type, "unary operator expected."); // will not throw
        std::unique_ptr<Expr> fact = Factor();
        return std::make_unique<UnaryExpr>(std::move(fact), op);
    }

    // INTEGER | STRING | "true" | "false"
    if (CurrTokIs(TokenType::INTEGER_VAL) || CurrTokIs(TokenType::STRING_VAL) || CurrTokIs(TokenType::TRUE) || CurrTokIs(TokenType::FALSE))
    {
        Advance(); // skip the value
        return std::make_unique<LiteralExpr>(std::move(GetPrevTok().lit));
    }

    // "(" expression ")"
    if (CurrTokIs(TokenType::LEFT_PAR))
    {
        Advance(); // skip the ')'
        std::unique_ptr<Expr> expr = Expression();
        Eat(TokenType::RIGHT_PAR, "')' expected after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }

    // IDENTIFIER
    if (GetCurrTok().type == TokenType::ID)
    {
        // functionExpr
        if (NextTokIs(TokenType::LEFT_PAR))
        {
            return FuncExpr();
        }

        // returns variable expression -> still may be a function call! -> interpreter handles this, parser cannot distinguish
        return std::make_unique<VariableExpr>(Eat(TokenType::ID, "identifier expected."));
    }

    throw Error(GetCurrTok().line_num, "expression expected.");
}

// functionExpr -> IDENTIFIER ("(" exprList ")")?;
std::unique_ptr<Expr> Parser::FuncExpr()
{
    Token id_token = Eat(TokenType::ID, "identifier expected.");
    std::vector<std::unique_ptr<Expr>> exprs{};

    if (!CurrTokIs(TokenType::LEFT_PAR)) // function can be called also without the () part (e.g. f() == f)
    {
        return std::make_unique<FunctionCallExpr>(std::move(exprs), id_token);
    }

    Advance(); // skip the '('

    // no expr list (i.e. empty)
    if (CurrTokIs(TokenType::RIGHT_PAR))
    {
        Advance(); // skip the ')'
        return std::make_unique<FunctionCallExpr>(std::move(exprs), id_token);
    }

    exprs = ExprList();

    Eat(TokenType::RIGHT_PAR, "')' expected.");

    return std::make_unique<FunctionCallExpr>(std::move(exprs), id_token);
}


// parameterList -> "(" (identifierList ":" type (";" identifierList ":" type)*)? ")";
std::vector<std::pair<Token, VariableType>> Parser::ParameterList()
{
    Eat(TokenType::LEFT_PAR, "'(' expected.");

    // if there is no identifier -> there must be right par.
    if (!CurrTokIs(TokenType::ID))
    {
        Eat(TokenType::RIGHT_PAR, "')' expected.");
        return {}; // return empty vector of parameters
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
            throw Error(GetCurrTok().line_num, "type expected.");
        }
        Advance(); // skip type

        // put all into parameter list
        for (auto&& identifier : identifiers)
        {
            parameter_list.push_back(std::make_pair(identifier, type));
        }

    } while (CurrMatchWith(TokenType::SEMICOLON));

    Eat(TokenType::RIGHT_PAR, "')' expected.");
    return parameter_list;
}

// identifierList -> IDENTIFIER ("," IDENTIFIER)*;
std::vector<Token> Parser::IdentifierList()
{
    std::vector<Token> tokens;
    tokens.push_back(Eat(TokenType::ID, "identifier expected."));
    while (CurrTokIs(TokenType::COMMA))
    {
        Advance(); // skip the comma
        tokens.push_back(Eat(TokenType::ID, "identifier expected."));
    }
    return tokens;
}

// statementList -> statement (";" statement)*;
std::vector<std::unique_ptr<Stmt>> Parser::StatementList()
{
    std::vector<std::unique_ptr<Stmt>> statement_list;
    statement_list.push_back(Statement());
    while (CurrTokIs(TokenType::SEMICOLON))
    {
        Advance(); // skip the semi
        statement_list.push_back(Statement());
    }
    return statement_list;
}

// exprList -> expression ("," expression)*;
std::vector<std::unique_ptr<Expr>> Parser::ExprList()
{
    std::vector<std::unique_ptr<Expr>> exprs;

    exprs.push_back(Expression());

    while (CurrTokIs(TokenType::COMMA))
    {
        Advance(); // skip the comma
        exprs.push_back(Expression());
    }
    return exprs;
}


Token& Parser::GetCurrTok()
{
    return tokens[curr_tok_num];
}

Token& Parser::GetPrevTok()
{
    return tokens[curr_tok_num - 1];
}


// check for type on current token
bool Parser::CurrTokIs(TokenType token_type)
{
    if (IsAtEnd())
    {
        return false;
    }
    return GetCurrTok().type == token_type;
}

// check for type on next token
bool Parser::NextTokIs(TokenType token_type)
{
    if (IsAtEnd())
    {
        return false;
    }
    return tokens[curr_tok_num + 1].type == token_type;
}


// checks for type among passed types and advances on match
bool Parser::CurrMatchWith(std::vector<TokenType> token_types)
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

// checks for type equality with passed type and advances on match
bool Parser::CurrMatchWith(TokenType token_type)
{
    if (GetCurrTok().type == token_type)
    {
        Advance();
        return true;
    }
    return false;
}


// advances if meets expected token and returns it, otherwise throws with passed message 
Token Parser::Eat(TokenType expected_type, std::string error_message)
{
    if (CurrTokIs(expected_type))
    {
        Advance();
        return GetPrevTok();
    }

    throw Error(GetCurrTok().line_num, error_message);
}

// move to next token
void Parser::Advance()
{
    if (!IsAtEnd())
    {
        curr_tok_num++;
    }
}

bool Parser::IsAtEnd()
{
    return GetCurrTok().type == TokenType::END_OF_FILE;
}