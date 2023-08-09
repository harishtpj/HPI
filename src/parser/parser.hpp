#pragma once

#include <vector>

#include "../Expr.hpp"
#include "../hpi.hpp"

using namespace std;

class ParseError : public runtime_error {
    public:
        Token token;
        ParseError(string msg, Token token);
};

class Parser {
    public:
        Parser(const vector<Token>& tokens);
        Expr* parse();
    
    private:
        const vector<Token> tokens;
        int current = 0;

        Expr* expression();
        Expr* equality();
        Expr* comparison();
        Expr* term();
        Expr* factor();
        Expr* unary();
        Expr* primary();

        bool match(const vector<TokenType>& types);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        Token consume(TokenType type, string message);
        ParseError error(Token token, string message);
        void synchronize();
};
