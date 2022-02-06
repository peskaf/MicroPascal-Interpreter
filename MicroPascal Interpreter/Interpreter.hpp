#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <memory>
#include <vector>

#include "Expr.hpp"
#include "Stmt.hpp"
#include "Environment.hpp"

class Interpreter : public VisitorExpr, public VisitorStmt
{
public:
	Interpreter();

	void Interpret(std::unique_ptr<Stmt> stmt);

private:
	Literal Visit(BinaryExpr& binExpr) override;

	Literal Visit(LiteralExpr& litExpr) override;

	Literal Visit(UnaryExpr& unExpr) override;

	Literal Visit(GroupingExpr& grExpr) override;
	
	Literal Visit(VariableExpr& varExpr) override;

	void Visit(WritelnStmt& writelnStmt) override;

	void Visit(CompoundStmt& compoundStmt) override;

	void Visit(ProgramStmt& programStmt) override;

	void Visit(EmptyStmt& emptyStmt) override;

	void Visit(VarDeclStmt& varDeclStmt) override;

	void Visit(FuncDeclStmt& funcDeclStmt) override;

	void Visit(AssignmentStmt& assignmentStmt) override;

	void Visit(IfStmt& ifStmt) override;

	void Visit(WhileStmt& whileStmt) override;

	void Visit(ForStmt& whileStmt) override;

	bool IsInt(Literal& lit);

	bool IsString(Literal& lit);

	bool IsBool(Literal& lit);

	std::string LitToString(Literal& lit);

	std::shared_ptr<Environment> global_env;
	std::shared_ptr<Environment> current_env;
};

#endif // !INTERPRETER_HPP