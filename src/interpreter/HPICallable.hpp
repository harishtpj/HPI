#pragma once

#include <vector>
#include <any>
#include <string>

using namespace std;

class Interpreter;

class HPICallable {
    public:
        virtual int arity() = 0;
        virtual any call(Interpreter& interpreter, vector<any> arguments) = 0;
        virtual string toString() = 0;

        virtual ~HPICallable() = default;
};