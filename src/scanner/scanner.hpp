#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "token.hpp" // using namespace std;

class Scanner {
    public:
        Scanner(string src);
        vector<Token> scanTokens();
    
    private:
        int start = 0;
        int current = 0;
        int line = 1;
        string src;
        vector<Token> tokens;
        unordered_map<string, TokenType> keywords;

        void scanToken();
        void scanIdentifier();
        void scanNumber();
        void scanString();
        bool isAtEnd();
        char advance();
        void addToken(TokenType type);
        void addToken(TokenType type,string literal);
        bool match(char expected);
        char peek();
        char peekNext();
        bool isAlpha(char c);
        bool isAlphaNumeric(char c);
        bool isDigit(char c);

};