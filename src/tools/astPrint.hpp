#include "../Expr.hpp"
#include "../scanner/token.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace HPI {
    class ASTPrinter : public ExprVisitor {
      public:
        string print(Expr* expr) {
            try {
                return std::any_cast<std::string>(expr->accept(this));
            } catch (const std::bad_any_cast &e) {
                std::cout << "Ast::print -> " << e.what() << "\n";
                return "something";
            }
        }
        any visitBinaryExpr(BinaryExpr* expr) override {
            return parenthesize(expr->Operator.lexeme,
                                {expr->left, expr->right});
        }
        any visitGroupingExpr(GroupingExpr* expr) override {
            return parenthesize("group", {expr->expression});
        }
        any visitLiteralExpr(LiteralExpr* expr) override {
            auto &type = expr->value.type();
            if (type == typeid(nullptr)) {
                std::string result{"nil"};
                return result;
            } else if (type == typeid(std::string)) {
                try {
                    return std::any_cast<std::string>(expr->value);
                } catch (const std::bad_any_cast &e) {
                    std::cout << "Ast::visitLiteralExpr -> " << e.what() << "\n";
                    return "error";
                }
            } else if (type == typeid(double)) {
                return std::to_string(std::any_cast<double>(expr->value));
            } else if (type == typeid(bool)) {
                if (std::any_cast<bool>(expr->value)) {
                    std::string result{"true"};
                    return result;
                } else {
                    std::string result{"false"};
                    return result;
                }
            } else {
                return "!!! no val match error";
            }
        }
        any visitUnaryExpr(UnaryExpr* expr) override {
            return parenthesize(expr->Operator.lexeme, {expr->right});
        }
        string parenthesize(std::string name, std::vector<Expr*> exprs) {
            std::ostringstream buffer;
            std::string pp = "(" + name;
            // print
            buffer << pp;
            for (auto expr : exprs) {
                buffer << " " << print(expr);
            }
            buffer << ")";
            return buffer.str();
        }
    };
} // namespace HPI

/// EXAMPLE USE:
// int main() {
//     std::unique_ptr<Expr> rootExpr(
//         new BinaryExpr(new UnaryExpr(*new Token(TokenType::MINUS, "-", "",
//         1),
//                                      new LiteralExpr("123")),
//                        *new Token(TokenType::STAR, "*", "", 1),
//                        new GroupingExpr(new LiteralExpr("45.67"))));
//     HPI::ASTPrinter pp;
//     pp.print(rootExpr.get());
//     std::cout << std::endl;
//     return 0;
// }