#pragma once

#include "HPICallable.hpp"
#include <chrono>
#include <sstream>
#include <iostream>

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
            return "<NativeFn clock>";
        }
};

class Read: public HPICallable {
    public:
        int arity() override {
            return 1;
        }

        any call(Interpreter& interpreter, vector<any> args) override {
            string prompt = any_cast<string>(args[0]);
            cout << prompt;

            string input;
            getline(cin, input);

            istringstream iss(input);
            double result;
            if (!(iss >> result)) {
                return input;
            }
            return result;
        }

        string toString() override {
            return "<NativeFn read>";
        }
};

class Exit: public HPICallable {
    public:
        short unsigned int paramCount;
        int arity() override {
            return paramCount;
        }

        any call(Interpreter& interpreter, vector<any> args) override {
            if (args.size() > 0)
                exit((int)any_cast<double>(args[0]));
            else
                exit(0);
        }

        string toString() override {
            return "<NativeFn exit>";
        }
};

template <typename... Args>
std::string format_helper(const std::string& format_string, std::vector<std::string>& values, Args... args) {
    if (values.empty()) {
        return std::format(format_string, args...);
    } else {
        std::string value = values.front();
        values.erase(values.begin());
        return format_helper(format_string, values, args..., value);
    }
}

class Fmt: public HPICallable {
    public:
        int paramCount;
        int arity() override {
            return paramCount;
        }

        any call(Interpreter& interpreter, vector<any> args) override {
            if (args.size() < 1) return string{""};
            string result = any_cast<std::string>(args[0]);
            args.erase(args.begin());
            size_t index = 0;

            for (const any& value : args) {
                size_t placeholder_pos = result.find("{}", index);

                if (placeholder_pos != string::npos) {
                    result.replace(placeholder_pos, 2, interpreter.stringify(value));
                    index = placeholder_pos + 1;
                } else {
                    break;  // No more placeholders found
                }
            }

            return result;
        }

        string toString() override {
            return "<NativeFn fmt>";
        }
};