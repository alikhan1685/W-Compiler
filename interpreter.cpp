#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cmath>
#include "Token.h"
#include "AST.h"
#include "SymbolTable.h"
using namespace std;
// Interpreter class: AST ko execute karta hai
// Ye program ke har statement ko evaluate karta hai aur variables ki values ko manage karta hai
class Interpreter {
public:
    // Constructor: AST aur symbol table ka reference leta hai
    Interpreter(ASTNodePtr tree, SymbolTable& symbols) : tree_(tree), symbols_(symbols) {}
    // interpret(): program execution start karta hai
    // Sab statements ko sequentially execute karta hai aur akhir mein variables ki values print karta hai
    void interpret() {
        cout << string(60, '=') << endl;
        cout << "EXECUTION PHASE" << endl;
        cout << string(60, '=') << endl;
        try {
            cout << "DEBUG: Tree has " << tree_->children.size() << " children" << endl;
            
            // Har child statement ko execute karo
            for (size_t i = 0; i < tree_->children.size(); i++) {
                cout << "\n--- Executing statement " << (i+1) << " ---" << endl;
                ASTNodePtr result = eval(tree_->children[i]);
                // Agar result INT ya STRING hai to print mat karo (variable reference already handle karega)
                if (result && (result->token.type == "INT" || result->token.type == "STRING")) {
                    // Kuch nahi karna
                }
            }  
            // Program ke end par sab variables ki values print karo
            cout << "\n" << string(60, '=') << endl;
            cout << "PROGRAM OUTPUT" << endl;
            cout << string(60, '=') << endl;
            symbols_.printAllValues();
            
        } catch (const exception& e) {
            cerr << "\nError: " << e.what() << endl;
        }
        cout << string(60, '=') << "\n" << endl;
    }
private:
    ASTNodePtr tree_;           // AST jo execute karna hai
    SymbolTable& symbols_;      // Variable values store karne ke liye symbol table
    // toNumber(): AST node ki value ko number (double) mein convert karta hai
    // Variable, INT, ya FLOAT ho sakta hai
    double toNumber(ASTNodePtr node) {
        if (!node) return 0;
        
        if (node->token.type == "VARIABLE") {
            Token val = symbols_.read(node->token.value);
            return stod(val.value);
        }
        if (node->token.type == "INT") {
            return stod(node->token.value);
        }
        if (node->token.type == "FLOAT") {
            return stod(node->token.value);
        }
        return 0;
    }
    // toBool(): AST node ki value ko boolean mein convert karta hai
    // BOOL literal, variable, ya numeric value (nonzero = true) ho sakta hai
    bool toBool(ASTNodePtr node) {
        if (!node) return false;
        
        if (node->token.type == "BOOL") {
            return node->token.value == "true";
        }
        if (node->token.type == "VARIABLE") {
            Token val = symbols_.read(node->token.value);
            if (val.type == "BOOL") return val.value == "true";
            return stod(val.value) != 0;
        }
        if (node->token.type == "INT") {
            return stod(node->token.value) != 0;
        }
        if (node->token.type == "FLOAT") {
            return stod(node->token.value) != 0;
        }
        return false;
    }
    // eval(): AST node ko evaluate (execute) karta hai
    // Ye interpreter ka core function hai - har tarah ke statement ko handle karta hai
    ASTNodePtr eval(ASTNodePtr node) {
        if (!node) {
            cout << "  [EVAL] Node is null" << endl;
            return nullptr;
        }
        // Debug print: node ki information show karo
        if (node->isList) {
            cout << "  [EVAL] List node, " << node->children.size() << " children" << endl;
            if (node->children.size() > 0 && !node->children[0]->isList) {
                cout << "  [EVAL] First child token: " << node->children[0]->token.type << endl;
            }
        } else {
            cout << "  [EVAL] Leaf node: " << node->token.type << " = " << node->token.value << endl;
        }
        // ===== DECLARATION (notekar var = value) =====
        // Pattern: [DECLARATION, variableName, value]
        if (node->isList && node->children.size() == 3) {
            auto& c = node->children;  
            if (c[0]->token.type == "DECLARATION") {
                string varName = c[1]->token.value;
                cout << "  [DECLARE] Processing declaration for: " << varName << endl;
                ASTNodePtr value = eval(c[2]);
                string valStr = value->token.value;
                cout << "  [DECLARE] Value = " << valStr << endl;      
                if (symbols_.exists(varName)) {
                    symbols_.write(varName, Token(value->token.type, valStr));
                    cout << "  [UPDATE] " << varName << " = " << valStr << endl;
                } else {
                    symbols_.declare(varName, Token(value->token.type, valStr), 0);
                    cout << "  [DECLARE] " << varName << " = " << valStr << endl;
                }
                return value;
            }
        }
        // ===== ASSIGNMENT (var = value) =====
        // Pattern: [VARIABLE, OPERATOR(=), value]
        if (node->isList && node->children.size() == 3) {
            auto& c = node->children;    
            if (c[0]->token.type == "VARIABLE" && c[1]->token.type == "OPERATOR" && c[1]->token.value == "=") {
                string varName = c[0]->token.value;
                cout << "  [ASSIGN] Processing assignment for: " << varName << endl;
                ASTNodePtr value = eval(c[2]);
                string valStr = value->token.value;
                cout << "  [ASSIGN] Value = " << valStr << endl;
                
                if (symbols_.exists(varName)) {
                    symbols_.write(varName, Token(value->token.type, valStr));
                    cout << "  [ASSIGN] " << varName << " = " << valStr << endl;
                } else {
                    symbols_.declare(varName, Token(value->token.type, valStr), 0);
                    cout << "  [DECLARE] " << varName << " = " << valStr << endl;
                }
                return value;
            }
        }
        // ===== BINARY OPERATIONS (+, -, *, /) =====
        // Pattern: [OPERATOR, leftOperand, rightOperand]
        if (node->isList && node->children.size() == 3) {
            auto op = node->children[0];
            auto left = node->children[1];
            auto right = node->children[2];
            if (op->token.type == "OPERATOR") {
                double a = toNumber(left);
                double b = toNumber(right);
                double result = 0;   
                if (op->token.value == "+") result = a + b;
                else if (op->token.value == "-") result = a - b;
                else if (op->token.value == "*") result = a * b;
                else if (op->token.value == "/") result = a / b;
                
                cout << "  [CALC] " << a << " " << op->token.value << " " << b << " = " << result << endl;
                
                // Agar result integer hai to INT return karo, nahi to FLOAT
                if (result == floor(result))
                    return make_shared<ASTNode>(Token("INT", to_string((int)result)));
                else
                    return make_shared<ASTNode>(Token("FLOAT", to_string(result)));
            }
            // ===== COMPARISON OPERATIONS (>, <, ==) =====
            if (op->token.type == "COMPARISON") {
                double a = toNumber(left);
                double b = toNumber(right);
                bool result = false;   
                if (op->token.value == ">") result = a > b;
                else if (op->token.value == "<") result = a < b;
                else if (op->token.value == "==") result = a == b;
                
                cout << "  [COMPARE] " << a << " " << op->token.value << " " << b << " = " << (result ? "true" : "false") << endl;
                return make_shared<ASTNode>(Token("BOOL", result ? "true" : "false"));
            }
        }
        // ===== IF-ELIF-ELSE =====
        // Pattern: [IF, condition, block, optional ELIF/ELSE blocks]
        if (node->isList && node->children.size() >= 2) {
            auto& c = node->children;
            if (c[0]->token.type == "IF") {
                cout << "  [IF] Processing IF statement" << endl;
                bool condition = toBool(eval(c[1]));
                if (condition) {
                    cout << "  [IF] Condition TRUE, executing block" << endl;
                    return eval(c[2]);
                }
                cout << "  [IF] Condition FALSE" << endl;
                // Check for ELIF and ELSE blocks
                for (size_t i = 3; i < c.size(); i++) {
                    auto& child = c[i];
                    if (child->isList && child->children.size() >= 2) {
                        auto& sub = child->children;
                        
                        if (sub[0]->token.type == "ELIF" || sub[0]->token.type == "ELSEIF") {
                            bool elifCond = toBool(eval(sub[1]));
                            if (elifCond) {
                                cout << "  [ELIF] Condition TRUE, executing block" << endl;
                                return eval(sub[2]);
                            }
                            cout << "  [ELIF] Condition FALSE" << endl;
                        }
                        else if (sub[0]->token.type == "ELSE") {
                            cout << "  [ELSE] Executing block" << endl;
                            return eval(sub[1]);
                        }
                    }
                }
                return nullptr;
            }
        }
        // ===== VARIABLE REFERENCE (Print) =====
        // Jab variable ka value read karna ho (jaise print statement mein)
        if (node->token.type == "VARIABLE") {
            Token val = symbols_.read(node->token.value);
            cout << "  [PRINT] " << node->token.value << " = " << val.value << endl;
            cout << val.value << endl;
            return make_shared<ASTNode>(val);
        }
        // ===== STRING LITERAL =====
        // Direct string print karna
        if (node->token.type == "STRING") {
            cout << "  [PRINT] String: " << node->token.value << endl;
            cout << node->token.value << endl;
            return node;
        }
        // ===== BLOCK =====
        // Statements ka group - sequentially execute karo
        if (node->isList && !node->children.empty()) {
            cout << "  [BLOCK] Executing block with " << node->children.size() << " statements" << endl;
            ASTNodePtr result = nullptr;
            for (auto& stmt : node->children) {
                result = eval(stmt);
            }
            return result;
        }
        return node;
    }
};