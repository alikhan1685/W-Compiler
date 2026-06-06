#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "lexer.cpp"
#include "Parser.cpp"
#include "interpreter.cpp"
using namespace std;
// Main function: Compiler ka entry point
// Ye program file ko read karta hai aur full compilation pipeline execute karta hai:
// Lexical Analysis -> Parsing -> Interpretation
int main() {
    cout << "\n   W++ COMPILER   \n" << endl;  // Compiler ka naam print karo
    // Source code file "wpp.txt" ko open karo
    ifstream file("wpp.txt");
    if (!file.is_open()) {                     // Agar file nahi mili to
        cerr << "Error: Cannot open wpp.txt" << endl;
        return 1;                              // Error ke saath program band karo
    }
    // File ki poori content memory mein read karo
    stringstream buffer;
    buffer << file.rdbuf();                    // File ka saara data buffer mein daalo
    string content = buffer.str();             // Buffer ko string mein convert karo
    file.close();                              // File ko close kardo
    try {                                      // Error handling start
        // PHASE 1: Lexical Analysis
        // Lexer source code ko tokens mein todta hai
        Lexer lexer(content);                  // Lexer ko source code do
        vector<Token> tokens = lexer.tokenize();   // Tokens generate karo
        // PHASE 2: Parsing (Syntactic Analysis)
        // Parser tokens se Abstract Syntax Tree (AST) banata hai
        Parser parser(tokens);                 // Parser ko tokens do
        ASTNodePtr ast = parser.parse();       // AST banao
        // Debug: Check karo ke AST mein kitne children hain
        cout << "DEBUG: AST has " << ast->children.size() << " children" << endl;
        // PHASE 3: Interpretation
        // SymbolTable variables ki values store karne ke liye
        SymbolTable symbols;                   // Symbol table banao
        Interpreter interpreter(ast, symbols); // Interpreter ko AST do
        interpreter.interpret();               // AST execute karo
        // Symbol table ki final state print karo (debugging ke liye)
        symbols.printSymbolTable();            // Sab variables ki values dikhao   
    } catch (const exception& e) {             // Agar koi error aaye to
        // Agar koi bhi phase fail ho jaye to error message dikhao
        cerr << "Compilation Failed: " << e.what() << endl;
    }   
    return 0;                                  // Program successfully end
}