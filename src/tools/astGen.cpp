#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace so_utils {
    static std::vector<std::string> split(const std::string& str,
                                          const std::string& delim) {
        std::vector<std::string> tokens;
        size_t prev = 0, pos = 0;
        do {
            pos = str.find(delim, prev);
            if (pos == std::string::npos)
                pos = str.length();
            auto token = str.substr(prev, pos - prev);
            if (!token.empty())
                tokens.push_back(token);
            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());
        return tokens;
    }
} // namespace so_utils

class ASTGenerator {
  public:
    using ASTSpecification = std::pair<std::string, std::vector<std::string>>;
    ASTGenerator(const std::string& aDir, const ASTSpecification aSpec)
        : outDir(aDir)
        , astSpec(aSpec) {}
    void generate() {
        defineAST();
    }
    void defineAST() {
        auto baseName = astSpec.first;
        auto path     = outDir + "/" + baseName + ".hpp";
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cout << "Unable to open file." << std::endl;
            return;
        }

        /// #ifndef guard
        file << "#pragma once" << std::endl;

        // Expr base abstract interface
        file << "#include \"scanner/token.hpp\"" << std::endl;
        file << "#include <vector>" << std::endl;
        file << ((baseName == "Stmt") ? "#include \"Expr.hpp\"" : " ") << std::endl;
        file << ((baseName == "Expr") ? "#include \"Stmt.hpp\"" : " ") << std::endl;

        // forward declarations
        file << "class " << baseName << "; // forward declare" << std::endl;
        for (auto type : astSpec.second) {
            auto className = type.substr(0, type.find(":"));
            file << "class " << className << "; // forward declare"
                 << std::endl;
        }

        defineVisitor(file, baseName);

        file << "class " << baseName << " {" << std::endl;
        file << "public:" << std::endl;
        file << "virtual ~" << baseName << "() {}" << std::endl;
        file << "virtual std::any accept(" << baseName + "Visitor* visitor) = 0;"
             << std::endl;
        file << "};" << std::endl;

        // Derived concrete classes
        for (auto type : astSpec.second) {
            auto className = type.substr(0, type.find(":"));
            auto fields    = type.substr(type.find(":") + 1, type.size());
            defineType(file, baseName, className, fields);
        }

        file.close();
    }
    void defineType(std::ofstream& file, const std::string& baseName,
                    const std::string& className, const std::string fields) {
        file << "class " + className + " : public " + baseName + " { "
             << std::endl;
        file << "public: " << std::endl;
        file << className + "(";
        auto fieldList = so_utils::split(fields, ",");
        bool first     = true;
        for (auto field : fieldList) {
            if (!first)
                file << ", ";
            if (first)
                first = false;
            auto fieldType = so_utils::split(field, " ")[0];
            auto fieldName = so_utils::split(field, " ")[1];
            if (!fieldType.compare(baseName)) {
                file << fieldType + "* " + fieldName;
            } else {
                file << fieldType + " " + fieldName;
            }
        }
        if (!fieldList.empty()) file << ")  : ";
        else file << ") ";
        first = true;
        for (auto field : fieldList) {
            if (!first)
                file << ", ";
            if (first)
                first = false;
            auto fieldName = so_utils::split(field, " ")[1];
            file << fieldName + "(" + fieldName + ")";
        }
        file << " {}" << std::endl;
        file << "std::any accept(" << baseName + "Visitor* visitor) override {"
             << std::endl;
        file << "return visitor->visit" << className << "(this);" << std::endl;
        file << "}" << std::endl;
        file << "public: " << std::endl;
        for (auto field : fieldList) {
            auto fieldType = so_utils::split(field, " ")[0];
            auto fieldName = so_utils::split(field, " ")[1];
            if (!fieldType.compare(baseName)) {
                file << fieldType + "* " + fieldName + ";" << std::endl;
            } else {
                file << fieldType + " " + fieldName + ";" << std::endl;
            }
        }
        file << "};" << std::endl;
    }
    void defineVisitor(std::ofstream& file, const std::string& baseName) {
        auto visitorClassName = baseName + "Visitor";
        file << "class " << visitorClassName << " {" << std::endl;
        file << "public:" << std::endl;
        file << "virtual ~" << visitorClassName << "() {}" << std::endl;
        for (auto type : astSpec.second) {
            auto className = type.substr(0, type.find(":"));
            file << "virtual std::any "
                 << "visit" + className << "(" << className << "* " << baseName
                 << ") = 0;" << std::endl;
        }
        file << "};" << std::endl;
    }

  private:
    const std::string outDir;
    const ASTSpecification astSpec;
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: astGen <output directory>" << std::endl;
        exit(64);
    } else {
        const std::string outDir                     = argv[1];
        const ASTGenerator::ASTSpecification astSpec = {"Expr", {
             "AssignExpr   : Token name, Expr value",
             "FunctionExpr : std::vector<Token> params, std::vector<Stmt*> body",
             "BinaryExpr   : Expr left, Token Operator, Expr right",
             "CallExpr     : Expr callee, Token paren, std::vector<Expr*> args",
             "GroupingExpr : Expr expression", 
             "LiteralExpr  : std::any value",
             "LogicalExpr  : Expr left, Token Operator, Expr right",
             "UnaryExpr    : Token Operator, Expr right",
             "VariableExpr : Token name",}};
        ASTGenerator exprAstGenerator(outDir, astSpec);
        exprAstGenerator.generate();

        const ASTGenerator::ASTSpecification astSpec2 = { "Stmt", {
            "BlockStmt      : std::vector<Stmt*> statements",
            "BreakStmt      :",
            "ExpressionStmt : Expr* expression",
            "FunctionStmt   : Token name, FunctionExpr* fn",
            "IfStmt         : Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
            "PrintStmt      : Expr* expression",
            "ReturnStmt     : Token keyword, Expr* value",
            "VarStmt        : Token name, Expr* initializer",
            "LoopStmt       : Stmt* body",}};
        ASTGenerator stmtAstGenerator(outDir, astSpec2);
        stmtAstGenerator.generate();
    }
    return 0;
}
