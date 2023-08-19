#pragma once

#include "HPICallable.hpp"
#include <chrono>

class Clock: public HPICallable {
    public:
        int arity() override {
            return 0;
        }

        any call(Interpreter& interpreter, vector<any> args) override {
            auto now = chrono::system_clock::now().time_since_epoch();
            return static_cast<double>(chrono::duration_cast<chrono::seconds>(now).count());
        }

        string toString() override {
            return "NativeFn clock";
        }
};