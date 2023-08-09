#pragma once

#include <string>
#include <format>

using namespace std;

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, COMMA, DOT, MINUS, PLUS,
    SLASH, STAR,
  
    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
  
    // Literals.
    IDENTIFIER, STRING, NUMBER,
  
    // Keywords.
    AND, ELSE, FALSE, FUNCTION, FOR, IF, NIL, OR,
    PRINT, PRINTLN, INPUT, RETURN, TRUE, WHILE, DO, END, TO, THEN,
  
    E_O_F
};

class Token {
    public:
        const TokenType type;
        const string lexeme;
        const string literal;
        const int line;

        Token(TokenType type, string lexeme, string literal, int line);
        string toString() const;
};