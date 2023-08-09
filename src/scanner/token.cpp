#include "token.hpp"

Token::Token(TokenType type, string lexeme, string literal, int line)
            : type(type),
              lexeme(lexeme), 
              literal(literal), 
              line(line) {}

string Token::toString() const {
    if (type == TokenType::STRING || type == TokenType::NUMBER) {
        return literal;
    }

    return lexeme;
}