#include <string>

using namespace std;

namespace HPI {
    void runFile(string path);
    void runPrompt();
    void run(string src);
    void error(int line, std::string msg);
    void report(int line, std::string where, std::string msg);
}