#include "parser.hpp"

ParseError::ParseError(string msg, Token token): runtime_error(msg), token(token) {}

Parser::Parser(const vector<Token>& tokens): tokens(tokens) {}

Expr* Parser::expression() {
    return equality();
}

Expr* Parser::equality() {
    Expr* expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        Expr* right = comparison();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr* Parser::comparison() {
    Expr* expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, 
                        TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        Expr* right = term();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr* Parser::term() {
    Expr* expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        Expr* right = factor();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr* Parser::factor() {
    Expr* expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        Expr* right = unary();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr* Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        Expr* right = unary();
        return new UnaryExpr(op, right);
    }

    return primary();
}

Expr* Parser::primary() {
    if (match({TokenType::FALSE})) return new LiteralExpr("false");
    if (match({TokenType::TRUE})) return new LiteralExpr("true");
    if (match({TokenType::NIL})) return new LiteralExpr("nil");

    if (match({TokenType::NUMBER, TokenType::STRING})) 
        return new LiteralExpr(previous().literal);

    if (match({TokenType::LEFT_PAREN})) {
        Expr* expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return new GroupingExpr(expr);
    }

    throw error(peek(), "Expect expression.");
}

Expr* Parser::parse() {
    try {
        return expression();
    } catch (ParseError error) {
        return nullptr;
    }
}

bool Parser::match(const vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::E_O_F;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::consume(TokenType type, string message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

ParseError Parser::error(Token token, string message) {
    HPI::error(token, message);
    return *new ParseError(message, token);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        switch (peek().type) {
            case TokenType::FUNCTION:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
        }

        advance();
    }
}
