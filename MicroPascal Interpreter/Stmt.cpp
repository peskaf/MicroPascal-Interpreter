#include "Stmt.hpp"

WritelnStmt::WritelnStmt(std::optional<std::unique_ptr<Expr>> m_expr) : expr(std::move(m_expr)) {};

void WritelnStmt::Accept(VisitorStmt& visitor)
{
	return visitor.Visit(*this);
};
