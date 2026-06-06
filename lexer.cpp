#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include "Token.h"
using namespace std;
// Lexer class: Source code ko tokens mein todta hai (lexical analysis)
// Ye program ke raw text ko chhotay chhotay meaningful units (tokens) mein convert karta hai
class Lexer {
private:
    string text;          // Input source code
    size_t idx;           // Current position in text
    char currentChar;     // Current character being processed
    int line;             // Current line number (error tracking ke liye)
    int column;           // Current column number (error tracking ke liye)
    vector<Token> tokens; // Extracted tokens ka list
public:
    // Constructor: Input string leta hai aur lexer ko initialize karta hai
    Lexer(string input) : text(input), idx(0), line(1), column(1) {
        currentChar = text.empty() ? '\0' : text[0];
    }
    // tokenize(): Main function jo source code ko tokens mein break karta hai
    // Whitespace, comments skip karta hai aur numbers, strings, identifiers, operators ko recognize karta hai
    vector<Token> tokenize() {
        cout << "\n" << string(60, '=') << endl;
        cout << "LEXICAL ANALYSIS PHASE" << endl;
        cout << string(60, '=') << endl;
        // Jab tak end of file nahi aa jata, characters process karte raho
        while (idx < text.size()) {
            // Whitespace ko skip karo (space, tab, newline)
            if (isspace(currentChar)) {
                if (currentChar == '\n') {
                    line++;      // Nai line start ho gayi
                    column = 1;
                } else {
                    column++;
                }
                move();
                continue;
            } 
            // Skip single-line comments (// se start hotay hain)
            if (currentChar == '/' && idx + 1 < text.size() && text[idx + 1] == '/') {
                while (idx < text.size() && currentChar != '\n') {
                    move();      // Line ke end tak skip karo
                }
                continue;
            } 
            // Number extract karo (integer ya float)
            if (isdigit(currentChar)) {
                tokens.push_back(extractNumber());
                continue;
            }
            // String literal extract karo (double quotes ke andar)
            if (currentChar == '"') {
                tokens.push_back(extractString());
                continue;
            }
            // Identifier ya keyword extract karo (variable names, if, else, etc.)
            if (isalpha(currentChar)) {
                tokens.push_back(extractIdentifier());
                continue;
            }
            // Operators handle karo (+, -, *, /, =, (, ), {, })
            if (string("+-*/=(){}").find(currentChar) != string::npos) {
                tokens.push_back(Token("OPERATOR", string(1, currentChar), line, column));
                cout << "  Token: OPERATOR    | Value: '" << currentChar << "'" << endl;
                move();
                continue;
            }
            // Comparison operators handle karo (>, <, ==, !=, etc.)
            if (string("><=?").find(currentChar) != string::npos) {
                tokens.push_back(extractComparison());
                continue;
            }
            // Agar koi unknown character ho to skip karo (error handling)
            move();
        }
        // End of file token add karo
        tokens.push_back(Token("EOF", "", line, column));
        
        cout << "\nTotal Tokens: " << tokens.size() << endl;
        cout << string(60, '=') << "\n" << endl;
        
        return tokens;
    }
private:
    // move(): Next character par move karta hai
    // Index increment karta hai aur currentChar update karta hai
    void move() {
        idx++;
        if (idx < text.size())
            currentChar = text[idx];
        else
            currentChar = '\0';
    }
    // extractNumber(): Number token extract karta hai
    // Integers aur floating point numbers dono ko handle karta hai
    Token extractNumber() {
        string num = "";
        bool isFloat = false;
        int startCol = column; 
        // Digits aur decimal point collect karo
        while (idx < text.size() && (isdigit(currentChar) || currentChar == '.')) {
            if (currentChar == '.') isFloat = true;
            num += currentChar;
            column++;
            move();
        }
        Token t = isFloat ? Token("FLOAT", num, line, startCol) : Token("INT", num, line, startCol);
        cout << "  Token: " << (isFloat ? "FLOAT" : "INT") << "        | Value: '" << num << "'" << endl;
        return t;
    }
    // extractString(): String literal extract karta hai
    // Double quotes ke andar ka content return karta hai
    Token extractString() {
        int startCol = column;
        move();                // Opening quote skip karo
        string str = "";
        // Jab tak closing quote ya newline nahi aa jata, characters collect karo
        while (idx < text.size() && currentChar != '"' && currentChar != '\n') {
            str += currentChar;
            column++;
            move();
        }
        // Closing quote skip karo
        if (currentChar == '"') {
            move();
            column++;
        }
        Token t = Token("STRING", str, line, startCol);
        cout << "  Token: STRING      | Value: '" << str << "'" << endl;
        return t;
    }
    // extractIdentifier(): Identifier ya keyword extract karta hai
    // Variable names, if, else, for, while, etc. ko recognize karta hai
    Token extractIdentifier() {
        string word = "";
        int startCol = column;
        // Letters, digits, underscore collect karo
        while (idx < text.size() && (isalnum(currentChar) || currentChar == '_')) {
            word += currentChar;
            column++;
            move();
        }
        // Check karo ke keyword hai ya variable
        string tokenType = "VARIABLE";
        auto it = KEYWORDS.find(word);
        if (it != KEYWORDS.end()) {
            tokenType = it->second;   // Keyword hai (IF, ELSE, etc.)
        }
        Token t = Token(tokenType, word, line, startCol);
        cout << "  Token: " << tokenType << "     | Value: '" << word << "'" << endl;
        return t;
    }
    // extractComparison(): Comparison operator extract karta hai
    // >, <, ==, !=, etc. ko handle karta hai
    Token extractComparison() {
        string comp = "";
        int startCol = column;
        // Comparison characters collect karo
        while (idx < text.size() && string("><=?").find(currentChar) != string::npos) {
            comp += currentChar;
            column++;
            move();
        }
        Token t = Token("COMPARISON", comp, line, startCol);
        cout << "  Token: COMPARISON  | Value: '" << comp << "'" << endl;
        return t;
    }
};