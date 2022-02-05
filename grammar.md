# MicroPascal - grammar

program -> "program" IDENTIFIER ";" declaration* compoundStmt "." EOF;

declaration -> procDecl | funcDecl | varDecl;

varDecl -> "var" (IDENTIFIER ("," IDENTIFIER)* ":" type ";")+;

funcDecl -> "function" IDENTIFIER parameterList? ":" type ";" declaration* compoundStmt;

procDecl -> "procedure" IDENTIFIER parameterList? ";" declaration* compoundStmt;

parameterList -> "(" (identifierList ":" type (";" identifierList ":" type)*)? ")";

identifierList -> IDENTIFIER ("," IDENTIFIER)*;

statementList -> statement (";" statement)*;

statement -> writelnStmt | procedureStmt | compoundStmt | ifStmt | forStmt | whileStmt | assignStmt | nothing;

functionExpr -> IDENTIFIER ("(" exprList ")")?;

procedureStmt -> IDENTIFIER ("(" exprList ")")?;

writelnStmt -> "writeln" "(" expression? ")";

ifStmt -> "if" expression "then" statement ("else" statement)?;

forStmt -> "for" assignStmt ("to" | "downto") expression "do" statement;

whileStmt -> "while" expression "do" statement;

assignStmt -> IDENTIFIER ":=" expression;

compoundStmt -> "begin" statementList "end";

exprList -> expression ("," expression)*;

expression -> simpleExpr ((">=" | "<=" | "<>" | "=" | ">" | "<") simpleExpr)?;

simpleExpr -> term (("+" | "-" | "or") term)*;

term -> factor (("*" | "div" | "and") factor)*;

factor -> ("+" | "-" | "not") factor | functionExpr | INTEGER | "(" expression ")" | "true" | "false" | STRING | CHAR;

nothing -> ε;

type -> "integer" | "string" | "boolean" | "char";