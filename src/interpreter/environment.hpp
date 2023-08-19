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
        any get(Token name);
        void assign(Token name, any value);
        void importMod(map<string, any> mod);

    private:
        map<string, any> values;
        Environment* enclosing;
};