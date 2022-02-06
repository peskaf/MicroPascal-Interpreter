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
    Token GetCurrTok();

    Token GetPrevious();

    Token Peek();

    void Advance();

    bool Check(TokenType token_type);

    bool IsAtEnd();

    bool NextMatchWith(std::vector<TokenType> token_types);

    Token Eat(TokenType expected_type, std::string error_message);

    std::unique_ptr<Expr> Expression();

    std::unique_ptr<Expr> FuncExpr();

    std::vector<std::unique_ptr<Expr>> ExprList();

    std::unique_ptr<Expr> SimpleExpr();

    std::unique_ptr<Expr> Term();

    std::unique_ptr<Expr> Factor();

    std::unique_ptr<Stmt> Statement();

    std::unique_ptr<Stmt> CompoundStatement();

    std::vector<std::unique_ptr<Stmt>> StatementList();

    std::unique_ptr<Stmt> Declaration();

    std::unique_ptr<Stmt> VarDecl();

    std::unique_ptr<Stmt> FuncDecl();

    std::unique_ptr<Stmt> ProcDecl();

    std::unique_ptr<Stmt> ProcStmt();

    std::vector<std::unique_ptr<Stmt>> Declarations();

    std::vector<std::pair<Token, VariableType>> ParameterList();

    std::unique_ptr<Stmt> WritelnStatement();

    std::unique_ptr<Stmt> Program();

    std::vector<Token> IdentifierList();

    std::unique_ptr<Stmt> EmptyStatement();

    std::unique_ptr<Stmt> AssignmentStatement();

    std::unique_ptr<Stmt> IfStatement();

    std::unique_ptr<Stmt> WhileStatement();

    std::unique_ptr<Stmt> ForStatement();

    std::vector<Token> tokens;
    int curr_tok_num = 0;
};

#endif // !PARSER_HPP
