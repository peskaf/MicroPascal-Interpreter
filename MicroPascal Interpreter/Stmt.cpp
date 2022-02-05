#include "Stmt.hpp"

ProgramStmt::ProgramStmt(std::string m_id, std::unique_ptr<Stmt> m_stmt, std::optional < std::vector<std::unique_ptr<Stmt>>> m_decl_stmts) : id(m_id), stmt(std::move(m_stmt)), decl_stmts(std::move(m_decl_stmts)) {};

void ProgramStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

CompoundStmt::CompoundStmt(std::vector<std::unique_ptr<Stmt>> m_statements) : statements(std::move(m_statements)) {};

void CompoundStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

WritelnStmt::WritelnStmt(std::optional<std::vector<std::unique_ptr<Expr>>> m_exprs) : exprs(std::move(m_exprs)) {};

void WritelnStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
};

EmptyStmt::EmptyStmt() {};

void EmptyStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

VarDeclStmt::VarDeclStmt(std::unordered_map<int, std::vector<Token>> m_variables) : variables(m_variables) {};

void VarDeclStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

AssignmentStmt::AssignmentStmt(Token m_token, std::unique_ptr<Expr> m_value) : token(m_token), value(std::move(m_value)) {};

void AssignmentStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

IfStmt::IfStmt(Token m_token, std::unique_ptr<Expr> m_condition, std::unique_ptr<Stmt> m_then_branch, std::optional<std::unique_ptr<Stmt>> m_else_branch) : token(m_token), condition(std::move(m_condition)), then_branch(std::move(m_then_branch)), else_branch(std::move(m_else_branch)) {};

void IfStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}

WhileStmt::WhileStmt(Token m_token, std::unique_ptr<Expr> m_condition, std::unique_ptr<Stmt> m_body) : token(m_token), condition(std::move(m_condition)), body(std::move(m_body)) {};

void WhileStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
}