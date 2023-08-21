#pragma once

#include <string>
#include <any>
#include <map>

#include "../scanner/token.hpp"

using namespace std;

class Environment {
    public:
        Environment();
        Environment(Environment* enclosing);
        
        void define(string name, any value);
        Environment* ancestor(int distance);
        any get(Token name);
        any getAt(int distance, string name);
        void assign(Token name, any value);
        void assignAt(int distance, Token name, any value);

    private:
        map<string, any> values;
        Environment* enclosing;
};