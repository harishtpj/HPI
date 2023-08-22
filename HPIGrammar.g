grammar HPIGrammar;

program        : declaration* EOF ;

declaration    : funDecl
               | varDecl
               | inputDecl
               | statement ;

funDecl        : ('function' | 'FUNCTION') function ;
function       : IDENTIFIER '(' parameters? ')' block ;
parameters     : IDENTIFIER (',' IDENTIFIER)* ;

varDecl        : ('let' | 'LET') IDENTIFIER ('=' expression)? ;
inputDecl      : ('input' | 'INPUT') IDENTIFIER ;

statement      : exprStmt
               | ifStmt
               | printStmt
               | printLnStmt
               | loopStmt
               | breakStmt
               | returnStmt
               | whileStmt
               | forStmt
               | block ;

breakStmt      : ('break' | 'BREAK') ;
returnStmt     : ('return' | 'RETURN') expression ;

block          : ('do' | 'DO') declaration* ('end' | 'END') ;

exprStmt       : expression ;
printStmt      : ('print' | 'PRINT') expression ;
printLnStmt    : ('println' | 'PRINTLN') expression ;

ifStmt         : ('if' | 'IF') expression ('then' | 'THEN') statement
                 (('else' | 'ELSE') statement)? ;

loopStmt        : ('loop' | 'LOOP') statement ;
whileStmt       : ('while' | 'WHILE')  expression statement;
forStmt         : ('for' | 'FOR') IDENTIFIER ('in' | 'IN') expression ('to' | 'TO') 
                  expression ('by' | 'BY') expression statement ;

expression     : assignment ;
assignment     : IDENTIFIER '=' assignment
               | logic_or ;
logic_or       : logic_and ('or' logic_and)* ;
logic_and      : equality ('and' equality)* ;
equality       : comparison ( ( '!=' | '==' ) comparison )* ;
comparison     : exponent ( ( '>' | '>=' | '<' | '<=' ) exponent )* ;
exponent       : term ( '**' term )* ;
term           : factor ( ( '-' | '+' ) factor )* ;
factor         : unary ( ( '/' | '*' | '%' ) unary )* ;
unary          : ( '!' | '-' ) unary | call ;
call           : primary ( '(' arguments? ')' )* ;
arguments      : expression ( ',' expression )* ;
primary        : ('true' | 'TRUE') | ('false' | 'FALSE') | ('nil' | 'NIL')
               | NUMBER | STRING
               | '(' expression ')' 
               | IDENTIFIER ;

NUMBER         : DIGIT+ ( '.' DIGIT+ )? ;
STRING         : ''' .+ ''' ;
IDENTIFIER     : ALPHA ( ALPHA | DIGIT )* ;
ALPHA          : ('a'..'z''A'..'Z''_')+ ;
DIGIT          : ('0'..'9')+ ;
