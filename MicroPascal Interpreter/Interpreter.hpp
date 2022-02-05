#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <memory>
#include <vector>

#include "Expr.hpp"
#include "Stmt.hpp"

class Interpreter : public VisitorExpr, public VisitorStmt
{
public:
	Literal Visit(BinaryExpr& binExpr) override;

	Literal Visit(LiteralExpr& litExpr) override;

	Literal Visit(UnaryExpr& unExpr) override;

	Literal Visit(GroupingExpr& grExpr) override;

	void Visit(WritelnStmt& writelnExpr) override;

	void Interpret(std::vector<std::unique_ptr<Stmt>> statements);

private:
	bool IsInt(Literal& lit);

	bool IsString(Literal& lit);

	bool IsBool(Literal& lit);

	std::string LitToString(Literal& lit);
};

#endif // !INTERPRETER_HPP