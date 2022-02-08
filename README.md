# Interpreter jazyka Pascal

## Specifikace

Podmnožina jazyka Pascal, pro kterou je interpreter napsán, je vymazena takto:
- proměnné typů integer, boolean, string,
- procedury, funkce,
- while cyklus, for cyklus,
- if ... then ... else,
- writeln,
- binární operátory +, -, *, /, <, >, <=, >=, <>, ==, and, or, unární - a not.

## Sestavení
- Naklonujte si tento repozitář.
### Windows
- V adresáři MicroPascal naleznete soubor MicroPascal.sln, otevřete jej ve Visual Studiu a projekt zkompilujte.
### Linux
- V hlavním adresáři repozitáře zavolejte `make`, projekt se zkompiluje pomocí gcc. Spoustitelný soubor naleznete v adresáři *build*.

## Použití (uživatelská dokumentace)

Předpokládá se použití z konzole (z adresáře se spustitelným souborem), na Windowsu jako příkaz `MicroPascal.exe file_name`, na Linuxu `./MicroPascal file_name`.

### Vstupní data
Vstupem programu je jméno souboru, který se bude interpretovat. Obsahem souboru by měl být textový zápis programu v programovacím jazyce Pascal, který je vymezen omezen na proměnné typů integer, string a boolean, for cyklus, while cyklus, if-then-else, writeln, procedury, funkce a základní binární a unární operátory (ne bitové operace a modulo). Komentáře k programu musí být obsaženy ve složených závorkách (`{ comment }`). Celková struktura očekávaného kódu je k nahlédnutí sekci Gramatika.

Je-li program špatně zapsán nebo pokud se ve vstupním souboru objevují neočekávané znaky, program odpoví chybovou hláškou. Ohlášena je vždy první chyba (ať už při skenování, parsování nebo interpretaci), na kterou se narazí, program spolu s ohlášením skončí.

Příklady vstupních dat (ať už validní či nevalidní) jsou k dispozici v adresáři `examples`.

## Popis programu (programátorská dokumentace)

Program je členěn do několika částí, konkrétně:

### Main
Vstupní bod celé aplikace, kontroluje správnost použití, načte data ze vstupního souboru a případně zachycuje výjimky.

### Lexer
Hlavním úkolem lexeru je čtení surového vstupního textu, na jehož základě vytváří tokeny (více v sekci Token), jedná se o takzvanou lexikální analýzu. Pokud lexer narazí na neznámý znak, tedy neexistuje Token, který by s daným symbolem mohl vytvořit, vyhodí výjimku. Lexer si zároveň hlídá tzv. *reserved keywords*, tedy klíčová slova, která má jazyk rezervovaná pro svou vlastní potřebu.

### Parser
Parser obdrží sekvenci tokenů od lexeru, ze které se snaží vytvořit tzv. *AST*, což je strom, který celý program reprezentuje. Pravidla parseru více méně odpovídají pravidlům gramatiky daného jazyka (zde Pascalu), jedná se o tzv. syntaktickou analýzu. Vstupním pravidlem gramatiky je pravidlo *program*. Vše ostatní v programu je nějakým způsobem dosažitelné právě z tohoto "uzlu". 

#### Gramatika

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

### Interpreter 
Interpreter obdrží AST (resp. ukazatel na uzel, který je vygenerován počátečním pravidlem gramatiky), který je zpracovaný parserem. Interpretace kódu je implementována pomocí [Visitor patternu](https://en.wikipedia.org/wiki/Visitor_pattern). Interpreter má pro každý druh uzlu, který parser vytvořil, definované, co má s daným uzlem udělat (jak ho vyhodnotit). Interpreter si hlídá i přetečení zásobníku volání, a to tak, že si počítá, kolikrát vstoupil do volání nějaké procedury/funkce. Tento počet snižuje při každém opuštění příslušného volání. V základu je maximální velikost zásobníku nastavena na 256. Interpreter zároveň obsluhuje globální environment (více v sekci Environment), ve kterém má definované globální proměnné, funkce a procedury. Hlídá také environment, který se právě používá, aby dané operace (přiřazení do proměnné apod.) uskutečnil ve správném prostředí.

### Token
Token je kompaktní reprezentací přečeného surového řetězce. Obsahuje právě přečtený (naskenovaný) řetězec (lexeme), typ daného tokenu, hodnotu tokenu (literál), která se využívá u datových typů, u ostatních obsahuje nullptr, a číslo řádku, na kterém příslušný Token ve vstupu nalezneme, což je velmi užitečné při hlášení chyb.

### Expression
Expression je vedle statementu (viz sekce Statement) jedním ze dvou hlavních "nadtypů" uzlů AST, který Parser vygeneruje. Expression se vyskytuje všude, kde je potřeba nějaká hodnota, a tedy lze vyhodnotit (ať už na číslo, na řetězec nebo pravdivostní hodnotu). V závislosti na typu expression (binární, unární, volání funkce,...) se prování při interpretaci různé činnosti, a to díky tomu, že má každá expression metodu Accept, která přijímá visitor, který má definované, jak s daným typem zacházet. Interpreter je tedy druh visitoru (který dědí právě od visitora pro expression a visitora pro statement).

### Statement
Statement na rozdíl od expression nevrazí nikdy hodnodtu, jen provede příslušnou akci, což může být například vypsání na výstup, while cyklus, deklarace funkce nebo celého programu či přiřazení do proměnné. Výhodou Pascalu je to, že žádný "uzel" AST nereprezentuje zároveň statement a expression, tedy mohou tyto dva koncepty fungovat odděleně.

### Environment
Environment, neboli prostředí, udává, jaké identifikátory jsou validní a k čemu jsou přiřazené. Může se jednat jak o volatené funckce nebo procedury, tak o literály, tedy celá čísla, logické hodnoty a řetězce. Environment se tedy stará o hlídání, zda v daném kontextu identifikátor existuje, zda má při přiřazení odpovídající typ, jako mu byl deklarován, nebo o samotnou deklaraci identifikátoru v příslušném prostředí. Environmenty si hlídají i tzv. "enclosing environment", což je vlastně nadřazené prostředí. Nejnadřazenějším prostředím je to globální, dále se uvažuje nové při deklaraci funkcí a procedur. Prostředí navzájem tedy tvoří vlastně stromovou strukturu. Pokud tedy není identifikátor validní v aktuálním prostředí, pokusí se program provést danou akci v tom nadřazeném atd.

V Pascalu obecně je třeba proměnné, funkce a procedury deklarovat předtím, než se příslušná proměnná, funkce nebo procedura používá. U proměnných jsem zvolil "null initialization", kterou používá např C#, tedy integer má hodnotu 0, string je prázdný řetězec a logická hodnota je *false*. Obecně u Pascalu ale platí, že obsah nepřiřazené proměnné není garantován (může obsahovat cokoliv).

## Ostatní

### Práce na projektu a subjektivní zhodnocení
Práce na interpreteru mě velmi bavila a mám pocit, že se jednalo prozatím o nejsmysluplnější projekt, na kterém jsem pracoval.
Naučil jsem se hodně jak o interpreterech, tak o programovacích jazycích a jejich designu. Mnohem lépe chápu návrh mj. i návrh syntaxe (a konceptů) jiných programovacích jazyků.

Velkou překážkou pro mne bylo, že jsem před zadáním projetu o tom, jak se interpreter vytváří, nic nevěděl, musel jsem si tedy mnoho věcí nastudovat.

### Zdroje
- NYSTROM, Robert. Crafting interpreters. [Místo vydání není známé]: Genever Benning, [2021]. ISBN 978-0-9905829-3-9.
- https://compilers.iecc.com/crenshaw/
- https://www.toptal.com/scala/writing-an-interpreter
