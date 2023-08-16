#pragma once

#include <string>
#include <any>
#include <format>

using namespace std;

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, COMMA, DOT, MINUS, PLUS,
    SLASH, STAR, LEFT_BRACKET, RIGHT_BRACKET,
  
    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
  
    // Literals.
    IDENTIFIER, STRING, NUMBER,
  
    // Keywords.
    AND, ELSE, FALSE, FUNCTION, FOR, IF, NIL, OR,
    PRINT, PRINTLN, INPUT, RETURN, TRUE, WHILE, DO, END, TO, 
    THEN, LET, BREAK, LOOP, BY, IN,

    NEWLINE,
    E_O_F
};

class Token {
    public:
        const TokenType type;
        const int line;
        const string lexeme;
        const any literal;

        Token(TokenType type, string lexeme, any literal, int line);
        string toString() const;
};