# MicroPascal Interpreter

## Syntax

MicroPascal is a subset of the Pascal programming language limited to
- variables of integer, boolean and string types,
- procedures and functions,
- while and for cycle,
- if-then-else statement,
- writeln statement,
- binary operators +, -, *, *div*, <, <=, >, =>, <>, =, :=, *and*, *or*,
- unary operators +, -, *not*.

## Build

1. clone the repository
2. call `make` in the main directory of the repo on Linux (compilation using gcc), executable file is in *build* directory; on Windows, you can compile the project using .sln file in MicroPascal directory

## Usage

Executing from console is expected, on Windows using `MicroPascal.exe file_name`, on Linux using `./MicroPascal file_name`.

### Input

Input of the program is a name of the file that is to be interpreted. The file is expected to contain program written in MicroPascal language, error messages are generated otherwise. Note that comments can be written only as `{ comment }`, not `(* comment *)`. In case of syntax uncertainty, see the Grammar section. Examples of both valid and invalid input files are present in the [examples](./examples) directory.

Note that only the first error encountered is being presented to the user in form of the error message.

## Grammar

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

## Resources
- Nystrom, R. (2021). Crafting Interpreters. Genever Benning.
