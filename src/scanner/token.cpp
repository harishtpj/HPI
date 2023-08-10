#include "token.hpp"

Token::Token(TokenType type, string lexeme, any literal, int line)
            : type(type),
              lexeme(lexeme), 
              literal(literal), 
              line(line) {}

string Token::toString() const {
    string literal_text;

    switch (type) {
      case (TokenType::IDENTIFIER):
        literal_text = lexeme;
        break;
      case (TokenType::STRING):
        literal_text = any_cast<string>(literal);
        break;
      case (TokenType::NUMBER):
        literal_text = to_string(any_cast<double>(literal));
        break;
      case (TokenType::TRUE):
        literal_text = "true";
        break;
      case (TokenType::FALSE):
        literal_text = "false";
        break;
      default:
        literal_text = "nil";
    }

    return lexeme + " " + literal_text;
  }