# MicroPascal - grammar

program -> "program" IDENTIFIER ";" declaration* compoundStmt "." EOF;


declaration -> procDecl | funcDecl | varDecl;

procDecl -> "procedure" IDENTIFIER parameterList? ";" declaration* compoundStmt ";";

funcDecl -> "function" IDENTIFIER parameterList? ":" type ";" declaration* compoundStmt ";";

varDecl -> "var" (identifierList ":" type ";")+;


statement -> writelnStmt | procedureStmt | compoundStmt | ifStmt | forStmt | whileStmt | assignStmt | emptyStmt;

writelnStmt -> "writeln" "(" exprList? ")";

procedureStmt -> IDENTIFIER ("(" exprList ")")?;

compoundStmt -> "begin" statementList "end";

ifStmt -> "if" expression "then" statement ("else" statement)?;

forStmt -> "for" assignStmt ("to" | "downto") expression "do" statement;

whileStmt -> "while" expression "do" statement;

assignStmt -> IDENTIFIER ":=" expression;

emptyStmt -> ε;


expression -> simpleExpr ((">=" | "<=" | "<>" | "=" | ">" | "<") simpleExpr)?;

simpleExpr -> term (("+" | "-" | "or") term)*;

term -> factor (("*" | "div" | "and") factor)*;

factor -> ("+" | "-" | "not") factor | functionExpr | INTEGER | "(" expression ")" | "true" | "false" | STRING | IDENTIFIER;

functionExpr -> IDENTIFIER ("(" exprList ")")?;


parameterList -> "(" (identifierList ":" type (";" identifierList ":" type)*)? ")";

identifierList -> IDENTIFIER ("," IDENTIFIER)*;

statementList -> statement (";" statement)*;

exprList -> expression ("," expression)*;


type -> "integer" | "string" | "boolean";