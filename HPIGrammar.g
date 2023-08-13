grammar HPIGrammar;

program        : declaration* EOF ;

declaration    : varDecl
               | statement ;

varDecl        : ('let' | 'LET') IDENTIFIER ('=' expression)? ;

statement      : exprStmt
               | printStmt
               | block ;

block          : '{' declaration* '}' ;

exprStmt       : expression ;
printStmt      : ('print' | 'PRINT') expression ;

expression     : assignment ;
assignment     : IDENTIFIER '=' assignment
               | equality ;
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