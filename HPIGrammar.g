grammar HPIGrammar;

expression      : literal
                | unary
                | binary
                | grouping ;

literal         : NUMBER | STRING | 'true' | 'false' | 'nil' ;
grouping        : '(' expression ')' ;
unary           : ( '-' | '!' ) expression ;
binary          : expression operator expression ;
operator        : '==' | '!=' | '<' | '<=' | '>' | '>='
                | '+'  | '-'  | '*' | '/' ;

NUMBER         : DIGIT+ ( '.' DIGIT+ )? ;
STRING         : '"' .+ '"' ;
IDENTIFIER     : ALPHA ( ALPHA | DIGIT )* ;
ALPHA          : ('a'..'z''A'..'Z''_')+ ;
DIGIT          : ('0'..'9')+ ;