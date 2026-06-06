#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include "Token.h"

using namespace std;

class Lexer {
private:
    string text;
    size_t idx;
    char currentChar;
    int line;
    int column;
    vector<Token> tokens;

public:
    Lexer(string input) : text(input), idx(0), line(1), column(1) {
        currentChar = text.empty() ? '\0' : text[0];
    }

    vector<Token> tokenize() {
        cout << "\n" << string(60, '=') << endl;
        cout << "LEXICAL ANALYSIS PHASE" << endl;
        cout << string(60, '=') << endl;
        
        while (idx < text.size()) {
            // Skip whitespace
            if (isspace(currentChar)) {
                if (currentChar == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                move();
                continue;
            }
            
            // Skip comments
            if ((currentChar == '/' && idx + 1 < text.size() && text[idx + 1] == '/') ||
                (currentChar == '#')) {
                while (idx < text.size() && currentChar != '\n') {
                    move();
                }
                continue;
            }
            
            // Check for #use directive
            if (currentChar == '#' && idx + 3 < text.size() && 
                text[idx + 1] == 'u' && text[idx + 2] == 's' && text[idx + 3] == 'e') {
                tokens.push_back(Token("USE", "#use", line, column));
                cout << "  📦 Token: USE         | Value: '#use'" << endl;
                for (int i = 0; i < 4; i++) {
                    move();
                    column++;
                }
                continue;
            }
            
            // Check for i/p and o/p keywords
            if (currentChar == 'i' && idx + 2 < text.size() && 
                text[idx + 1] == '/' && text[idx + 2] == 'p') {
                tokens.push_back(Token("INPUT", "i/p", line, column));
                cout << "  📥 Token: INPUT       | Value: 'i/p'" << endl;
                move(); move(); move();
                column += 3;
                continue;
            }
            
            if (currentChar == 'o' && idx + 2 < text.size() && 
                text[idx + 1] == '/' && text[idx + 2] == 'p') {
                tokens.push_back(Token("OUTPUT", "o/p", line, column));
                cout << "  📤 Token: OUTPUT      | Value: 'o/p'" << endl;
                move(); move(); move();
                column += 3;
                continue;
            }
            
            // Numbers
            if (isdigit(currentChar)) {
                tokens.push_back(extractNumber());
                continue;
            }

            // Strings
            if (currentChar == '"') {
                tokens.push_back(extractString());
                continue;
            }

            // Identifiers and Keywords
            if (isalpha(currentChar)) {
                tokens.push_back(extractIdentifier());
                continue;
            }

            // Operators
            if (string("+-*/=(){}.").find(currentChar) != string::npos) {
                tokens.push_back(Token("OPERATOR", string(1, currentChar), line, column));
                cout << "  🔧 Token: OPERATOR    | Value: '" << currentChar << "'" << endl;
                move();
                continue;
            }

            // Comparison operators
            if (string("><=!").find(currentChar) != string::npos) {
                tokens.push_back(extractComparison());
                continue;
            }

            move();
        }

        tokens.push_back(Token("EOF", "", line, column));
        
        cout << "\nTotal Tokens: " << tokens.size() << endl;
        cout << string(60, '=') << "\n" << endl;
        
        return tokens;
    }

private:
    void move() {
        idx++;
        if (idx < text.size())
            currentChar = text[idx];
        else
            currentChar = '\0';
    }

    Token extractNumber() {
        string num = "";
        bool isFloat = false;
        int startLine = line;
        int startCol = column;
        
        while (idx < text.size() && (isdigit(currentChar) || currentChar == '.')) {
            if (currentChar == '.') {
                if (isFloat) {
                    cout << "  WARNING: Multiple decimal points in number" << endl;
                    break;
                }
                isFloat = true;
            }
            num += currentChar;
            column++;
            move();
        }
        
        Token t = isFloat ? Token("FLOAT", num, startLine, startCol) 
                          : Token("INT", num, startLine, startCol);
        cout << "  🔢 Token: " << (isFloat ? "FLOAT" : "INT") << "         | Value: '" << num << "'" << endl;
        return t;
    }

    Token extractString() {
        int startLine = line;
        int startCol = column;
        move(); // Skip opening quote
        string str = "";
        
        while (idx < text.size() && currentChar != '"' && currentChar != '\n') {
            str += currentChar;
            column++;
            move();
        }
        
        if (currentChar == '"') {
            move();
            column++;
        } else {
            cout << "  WARNING: Unterminated string" << endl;
        }
        
        Token t = Token("STRING", str, startLine, startCol);
        cout << "  📝 Token: STRING      | Value: '" << str << "'" << endl;
        return t;
    }

    Token extractIdentifier() {
        string word = "";
        int startLine = line;
        int startCol = column;
        
        while (idx < text.size() && (isalnum(currentChar) || currentChar == '_')) {
            word += currentChar;
            column++;
            move();
        }
        
        string tokenType = "VARIABLE";
        
        // Check if it's a keyword
        auto it = KEYWORDS.find(word);
        if (it != KEYWORDS.end()) {
            tokenType = it->second;
        }
        
        Token t = Token(tokenType, word, startLine, startCol);
        
        string icon = "📝";
        if (tokenType == "DECLARATION") icon = "📦";
        else if (tokenType == "ASSIGN") icon = "✏️";
        else if (tokenType == "IF") icon = "❓";
        else if (tokenType == "WHILE") icon = "🔄";
        else if (tokenType == "MAIN") icon = "🏠";
        else if (tokenType == "MATH") icon = "📐";
        else if (tokenType == "USE") icon = "📚";
        else if (tokenType == "SQRT") icon = "√";
        else if (tokenType == "POW") icon = "🔢";
        else if (tokenType == "CEIL") icon = "⬆️";
        else if (tokenType == "FLOOR") icon = "⬇️";
        else if (tokenType == "FABS") icon = "⎪";
        else if (tokenType == "FACTORIAL") icon = "!";
        
        cout << "  " << icon << " Token: " << tokenType << "   | Value: '" << word << "'" << endl;
        return t;
    }

    Token extractComparison() {
        string comp = "";
        int startLine = line;
        int startCol = column;
        
        while (idx < text.size() && string("><=!").find(currentChar) != string::npos) {
            comp += currentChar;
            column++;
            move();
        }
        
        Token t = Token("COMPARISON", comp, startLine, startCol);
        cout << "  ⚖️  Token: COMPARISON | Value: '" << comp << "'" << endl;
        return t;
    }
};