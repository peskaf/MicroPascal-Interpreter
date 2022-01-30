# MicroPascal - grammar

program -> "program" IDENTIFIER ";" declaration* compoundStmt "." EOF;

declaration -> procDecl | funcDecl | varDecl | nothing;

varDecl -> "var" (IDENTIFIER ("," IDENTIFIER)* ":" type ";")+;

funcDecl -> "function" IDENTIFIER parameterList? ":" type ";" declaration compoundStmt;

procDecl -> "procedure" IDENTIFIER parameterList? ";" declaration compoundStmt;

parameterList -> "(" identifierList ":" type (";" identifierList ":" type)* ")";

identifierList -> IDENTIFIER ("," IDENTIFIER)*;

statementList -> statement (";" statement)*;

statement -> writelnStmt | procedureStmt | functionStmt | compoundStmt | ifStmt | forStmt | whileStmt | assignStmt | nothing;

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

term -> factor (("*" | "div" | "and") factor)*;

factor -> functionStmt | INTEGER | "(" expression ")" | "not" factor | "true" | "false" | STRING | CHAR;

nothing -> ;

type -> "integer" | "string" | "boolean" | "char";


---------------------------------------------------------------------------------------------------

# Grammar version for https://bnfplayground.pauliankline.com/

<program> ::= "program" <IDENTIFIER> ";" <declaration>* <compoundStmt> "."

<declaration> ::= <procDecl> | <funcDecl> | <varDecl> | <nothing>

<varDecl> ::= "var" (<IDENTIFIER> ("," <IDENTIFIER>)* ":" <type> ";")+

<funcDecl> ::= "function" <IDENTIFIER> <parameterList>? ":" <type> ";" <declaration> <compoundStmt>

<procDecl> ::= "procedure" <IDENTIFIER> <parameterList>? ";" <declaration> <compoundStmt>

<parameterList> ::= "(" <identifierList> ":" <type> (";" <identifierList> ":" <type>)* ")"

<identifierList> ::= <IDENTIFIER> ("," <IDENTIFIER>)*

<statementList> ::= <statement> (";" <statement>)*

<statement> ::= <writelnStmt> | <procedureStmt> | <functionStmt> | <compoundStmt> | <ifStmt> | <forStmt> | <whileStmt> | <assignStmt> | <nothing>

<functionStmt> ::= <IDENTIFIER> ("(" <expressionList> ")")?

<procedureStmt> ::= <IDENTIFIER> ("(" <expressionList> ")")?

<writelnStmt> ::= "writeln" "(" <expression> ")"

<ifStmt> ::= "if" <expression> "then" <statement> ("else" <statement>)?

<forStmt> ::= "for" <assignStmt> ("to" | "downto") <expression> "do" <statement>

<whileStmt> ::= "while" <expression> "do" <statement>

<assignStmt> ::= <IDENTIFIER> ":=" <expression>

<compoundStmt> ::= "begin" <statementList> "end"

<expressionList> ::= <expression> ("," <expression>)*

<expression> ::= <simpleExpression> ((">=" | "<=" | "<>" | "=" | ">" | "<") <simpleExpression>)*

<simpleExpression> ::= ("+" | "-")? <term> (("+" | "-" | "or") <term>)*

<term> ::= <factor> (( "*" | "div" | "and") <factor>)*

<factor> ::= <functionStmt> | <INTEGER> | "(" <expression> ")" | "not" <factor> | "true" | "false" | <STRING> | <CHAR>

<nothing> ::= "_"

<type> ::= "integer" | "string" | "boolean" | "char"

<IDENTIFIER> ::= "x"

<INTEGER> ::= "145" | "6" | "77"

<CHAR> ::= "a" | "b" | "g"

<STRING> ::= "hello" | "world" | "salvator"
