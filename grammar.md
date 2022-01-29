# MicroPascal - grammar

program -> "program" IDENTIFIER ";" declarations compoundStmt "." EOF;
declarations -> procDecl | funcDecl | varDecl | nothing;
varDecl -> "var" (IDENTIFIER ("," IDENTIFIER)* ":" type ";")+;
funcDecl -> "function" IDENTIFIER parameterList? ":" type ";" declarations compoundStatement;
procDecl -> "procedure" IDENTIFIER parameterList? ";" declarations compoundStatement;
parameterList -> "(" IDENTIFIER ":" type ("," IDENTIFIER ":" type)* ")";
statementList -> statement (";" statement)* (";")?; // beware of the semi
statement -> writelnStmt | procedureStmt | functionStmt | compundStmt | ifStmt | forStmt | whileStmt | assignStmt | nothing;
functionStmt -> IDENTIFIER ("(" expressionList")")?;
procedureStmt -> IDENTIFIER ("(" expressionList")")?;
writelnStmt -> "writeln" "(" expression ")";
ifStmt -> "if" expression "then" statement ("else" statement)?;
forStmt -> "for" assignStmt ("to" | "downto") expression "do" statement;
whileStmt -> "while" expression "do" statement;
assignStmt -> IDENTIFIER ":=" expression;
compoundStmt -> "begin" statementList "end";
expressionList -> expression ("," expression)*;
expression -> simpleExpression ((">=" | "<=" | "<>" | "=" | ">" | "<") simpleExpression)*;
simpleExpression -> ("+" | "-")? term (("+" | "-" | "or") term)*;
term -> factor ( "*" | "div" | "and") factor)*;
factor -> functionStmt | INTEGER | "(" expression ")" | "not" factor | "true" | "false" | STRING | CHAR;
nothing -> ;
type -> "integer" | "string" | "boolean" | "char";
