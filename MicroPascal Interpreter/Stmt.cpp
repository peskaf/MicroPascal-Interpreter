#include "Stmt.hpp"

ProgramStmt::ProgramStmt(Token m_id, std::unique_ptr<Stmt> m_stmt) : id(m_id), stmt(std::move(m_stmt)) {};

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