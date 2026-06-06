#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "lexer.cpp"
#include "Parser.cpp"
#include "interpreter.cpp"
#include "SymbolTable.h"

using namespace std;

void printHeader() {
    cout << "\n";
    cout << "============================================================" << endl;
    cout << "                    W++ COMPILER v2.0                       " << endl;
    cout << "           Interactive Mode - Type Code Directly            " << endl;
    cout << "============================================================" << endl;
    cout << "\n";
    cout << "Usage:\n";
    cout << "  #use math              - Load math library\n";
    cout << "  main() { ... }         - Main function\n";
    cout << "  let x = value          - Declare variable\n";
    cout << "  set x = value          - Assign variable\n";
    cout << "  o/p expression         - Print output\n";
    cout << "  i/p variable           - Take input\n";
    cout << "  check condition {...}  - If statement\n";
    cout << "  end                    - End program\n";
    cout << "\n";
    cout << "Commands:\n";
    cout << "  > run      - Execute the code\n";
    cout << "  > clear    - Clear code buffer\n";
    cout << "  > show     - Show current code\n";
    cout << "  > exit     - Exit compiler\n";
    cout << "============================================================\n";
}

int main() {
    printHeader();
    
    vector<string> codeLines;
    string line;
    bool running = true;
    
    while (running) {
        cout << "\nW++ > ";
        getline(cin, line);
        
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line == "exit" || line == "quit") {
            cout << "Goodbye!\n";
            running = false;
            break;
        }
        else if (line == "run" || line == "r") {
            if (codeLines.empty()) {
                cout << "No code to execute!\n";
                continue;
            }
            
            string code;
            for (const auto& cl : codeLines) code += cl + "\n";
            
            cout << "\n" << string(60, '=') << endl;
            cout << "EXECUTING CODE" << endl;
            cout << string(60, '=') << endl;
            cout << "\nCode:\n";
            for (size_t i = 0; i < codeLines.size(); i++) {
                cout << "  " << (i+1) << " | " << codeLines[i] << endl;
            }
            cout << "\n";
            
            try {
                Lexer lexer(code);
                vector<Token> tokens = lexer.tokenize();
                Parser parser(tokens);
                ASTNodePtr ast = parser.parse();
                SymbolTable symbols;
                Interpreter interpreter(ast, symbols);
                interpreter.interpret();
                symbols.printSymbolTable();
                cout << "\n✓ Execution completed!\n";
            } catch (const exception& e) {
                cout << "\n✗ Error: " << e.what() << endl;
            }
            
            cout << "\nType 'clear' to write new code, or continue adding lines: ";
            continue;
        }
        else if (line == "clear" || line == "c") {
            codeLines.clear();
            cout << "Code buffer cleared!\n";
        }
        else if (line == "show" || line == "s") {
            if (codeLines.empty()) {
                cout << "No code in buffer.\n";
            } else {
                cout << "\nCurrent Code:\n" << string(40, '-') << endl;
                for (size_t i = 0; i < codeLines.size(); i++) {
                    cout << (i+1) << ". " << codeLines[i] << endl;
                }
                cout << string(40, '-') << endl;
            }
        }
        else if (line.empty()) {
            continue;
        }
        else {
            codeLines.push_back(line);
            cout << "Line added. Type 'run' to execute.\n";
        }
    }
    
    return 0;
}