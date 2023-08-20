#pragma once

#include "interpreter.hpp"

class HPIFunction: public HPICallable {
    public:
        HPIFunction(string name, FunctionExpr* declaration, Environment* closure);
        int arity();
        any call(Interpreter& interpreter, vector<any> arguments);
        string toString();

    private:
        string name;
        FunctionExpr* declaration;
        Environment* closure;
};