#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <string>

#include "Expr.hpp"
#include "Stmt.hpp"
#include "TokenType.hpp"
#include "Token.hpp"

class Parser // syntactic analysis, creates AST, contains grammar rules
{
public:
    Parser(std::vector<Token>& m_tokens);

    std::unique_ptr<Stmt> Parse();

private:
    std::unique_ptr<Stmt> Program();

    std::vector<std::shared_ptr<Stmt>> Declarations();
    std::shared_ptr<Stmt> Declaration();
    std::shared_ptr<Stmt> ProcDecl();
    std::shared_ptr<Stmt> FuncDecl();
    std::shared_ptr<Stmt> VarDecl();

    std::unique_ptr<Stmt> Statement();
    std::unique_ptr<Stmt> WritelnStatement();
    std::unique_ptr<Stmt> ProcStmt();
    std::unique_ptr<Stmt> CompoundStatement();
    std::unique_ptr<Stmt> IfStatement();
    std::unique_ptr<Stmt> ForStatement();
    std::unique_ptr<Stmt> WhileStatement();
    std::unique_ptr<Stmt> AssignmentStatement();
    std::unique_ptr<Stmt> EmptyStatement();

    std::unique_ptr<Expr> Expression();
    std::unique_ptr<Expr> SimpleExpr();
    std::unique_ptr<Expr> Term();
    std::unique_ptr<Expr> Factor();
    std::unique_ptr<Expr> FuncExpr();

    std::vector<std::pair<Token, VariableType>> ParameterList();
    std::vector<Token> IdentifierList();
    std::vector<std::unique_ptr<Stmt>> StatementList();
    std::vector<std::unique_ptr<Expr>> ExprList();


    Token& GetCurrTok();
    Token& GetPrevTok();

    bool CurrTokIs(TokenType token_type);
    bool NextTokIs(TokenType token_type);

    bool CurrMatchWith(std::vector<TokenType> token_types);
    bool CurrMatchWith(TokenType token_type);

    Token Eat(TokenType expected_type, std::string error_message);
    void Advance();
    bool IsAtEnd();
    

    std::vector<Token> tokens;
    int curr_tok_num = 0;
};

#endif // !PARSER_HPP