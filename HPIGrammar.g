grammar HPIGrammar;

program        : declaration* EOF ;

declaration    : varDecl
               | statement ;

varDecl        : ('let' | 'LET') IDENTIFIER ('=' expression)? ;

statement      : exprStmt
               | ifStmt
               | printStmt
               | printLnStmt
               | loopStmt
               | whileStmt
               | forStmt
               | block ;

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
comparison     : term ( ( '>' | '>=' | '<' | '<=' ) term )* ;
term           : factor ( ( '-' | '+' ) factor )* ;
factor         : unary ( ( '/' | '*' ) unary )* ;
unary          : ( '!' | '-' ) unary
               | primary ;
primary        : ('true' | 'TRUE') | ('false' | 'FALSE') | ('nil' | 'NIL')
               | NUMBER | STRING
               | '(' expression ')' 
               | IDENTIFIER ;

NUMBER         : DIGIT+ ( '.' DIGIT+ )? ;
STRING         : ''' .+ ''' ;
IDENTIFIER     : ALPHA ( ALPHA | DIGIT )* ;
ALPHA          : ('a'..'z''A'..'Z''_')+ ;
DIGIT          : ('0'..'9')+ ;