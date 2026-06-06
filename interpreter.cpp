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

class Interpreter {
public:
    Interpreter(ASTNodePtr tree, SymbolTable& symbols) : tree_(tree), symbols_(symbols) {}

    void interpret() {
        cout << string(60, '=') << endl;
        cout << "EXECUTION PHASE" << endl;
        cout << string(60, '=') << endl;
        
        try {
            for (size_t i = 0; i < tree_->children.size(); i++) {
                cout << "\n--- Processing statement " << (i+1) << " ---" << endl;
                ASTNodePtr result = eval(tree_->children[i]);
                if (result && result->token.type == "END") {
                    break;
                }
            }
            
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
    ASTNodePtr tree_;
    SymbolTable& symbols_;

    // Math functions
    double factorial(double n) {
        if (n < 0) throw runtime_error("Factorial of negative number is not defined");
        if (n == 0 || n == 1) return 1;
        if (n != floor(n)) throw runtime_error("Factorial is only defined for integers");
        double result = 1;
        for (int i = 2; i <= (int)n; i++) result *= i;
        return result;
    }
    
    ASTNodePtr evalMathFunction(ASTNodePtr node) {
        if (!node->isList || node->children.size() < 2) return nullptr;
        auto& children = node->children;
        
        if (children[1]->token.type == "SQRT") {
            double arg = toNumber(eval(children[2]));
            if (arg < 0) throw runtime_error("sqrt() of negative number");
            double result = sqrt(arg);
            return make_shared<ASTNode>(Token(result == floor(result) ? "INT" : "FLOAT", to_string(result)));
        }
        if (children[1]->token.type == "POW") {
            double base = toNumber(eval(children[2]));
            double exp = toNumber(eval(children[3]));
            double result = pow(base, exp);
            return make_shared<ASTNode>(Token(result == floor(result) ? "INT" : "FLOAT", to_string(result)));
        }
        if (children[1]->token.type == "CEIL") {
            double arg = toNumber(eval(children[2]));
            double result = ceil(arg);
            return make_shared<ASTNode>(Token("INT", to_string((int)result)));
        }
        if (children[1]->token.type == "FLOOR") {
            double arg = toNumber(eval(children[2]));
            double result = floor(arg);
            return make_shared<ASTNode>(Token("INT", to_string((int)result)));
        }
        if (children[1]->token.type == "FABS") {
            double arg = toNumber(eval(children[2]));
            double result = fabs(arg);
            return make_shared<ASTNode>(Token(result == floor(result) ? "INT" : "FLOAT", to_string(result)));
        }
        if (children[1]->token.type == "FACTORIAL") {
            double arg = toNumber(eval(children[2]));
            double result = factorial(arg);
            return make_shared<ASTNode>(Token("INT", to_string((int)result)));
        }
        return nullptr;
    }

    string getStringValue(ASTNodePtr node) {
        if (!node) return "";
        if (node->token.type == "VARIABLE") return symbols_.read(node->token.value).value;
        return node->token.value;
    }

    bool isNumeric(const string& str) {
        if (str.empty()) return false;
        bool hasDecimal = false;
        for (size_t i = 0; i < str.length(); i++) {
            if (i == 0 && str[i] == '-') continue;
            if (str[i] == '.') {
                if (hasDecimal) return false;
                hasDecimal = true;
                continue;
            }
            if (!isdigit(str[i])) return false;
        }
        return true;
    }

    double toNumber(ASTNodePtr node) {
        if (!node) return 0;
        if (node->token.type == "VARIABLE") return stod(symbols_.read(node->token.value).value);
        return stod(node->token.value);
    }
    
    bool toBool(ASTNodePtr node) {
        if (!node) return false;
        if (node->token.type == "VARIABLE") {
            string val = symbols_.read(node->token.value).value;
            if (isNumeric(val)) return stod(val) != 0;
            return !val.empty();
        }
        if (node->token.type == "INT") return stod(node->token.value) != 0;
        if (node->token.type == "STRING") return !node->token.value.empty();
        return false;
    }

    ASTNodePtr eval(ASTNodePtr node) {
        if (!node) return nullptr;
        
        // Math function call
        if (node->isList && node->children.size() >= 2 && node->children[0]->token.type == "MATH") {
            return evalMathFunction(node);
        }
        
        // Declaration (let)
        if (node->isList && node->children.size() == 3 && node->children[0]->token.type == "DECLARATION") {
            string varName = node->children[1]->token.value;
            ASTNodePtr value = eval(node->children[2]);
            if (symbols_.exists(varName)) {
                symbols_.write(varName, value->token);
            } else {
                symbols_.declare(varName, value->token, 0);
            }
            return value;
        }
        
        // Assignment (set)
        if (node->isList && node->children.size() == 3 && node->children[0]->token.type == "VARIABLE") {
            string varName = node->children[0]->token.value;
            ASTNodePtr value = eval(node->children[2]);
            if (symbols_.exists(varName)) {
                symbols_.write(varName, value->token);
            } else {
                symbols_.declare(varName, value->token, 0);
            }
            return value;
        }
        
        // Binary operations (+, -, *, /)
        if (node->isList && node->children.size() == 3) {
            auto op = node->children[0];
            auto left = node->children[1];
            auto right = node->children[2];
            
            if (op->token.type == "OPERATOR") {
                if (op->token.value == "+") {
                    string l = getStringValue(left);
                    string r = getStringValue(right);
                    if (isNumeric(l) && isNumeric(r)) {
                        double result = stod(l) + stod(r);
                        return make_shared<ASTNode>(Token(result == floor(result) ? "INT" : "FLOAT", to_string(result)));
                    } else {
                        return make_shared<ASTNode>(Token("STRING", l + r));
                    }
                }
                else if (op->token.value == "-") {
                    double result = toNumber(left) - toNumber(right);
                    return make_shared<ASTNode>(Token(result == floor(result) ? "INT" : "FLOAT", to_string(result)));
                }
                else if (op->token.value == "*") {
                    double result = toNumber(left) * toNumber(right);
                    return make_shared<ASTNode>(Token(result == floor(result) ? "INT" : "FLOAT", to_string(result)));
                }
                else if (op->token.value == "/") {
                    double result = toNumber(left) / toNumber(right);
                    return make_shared<ASTNode>(Token(result == floor(result) ? "INT" : "FLOAT", to_string(result)));
                }
            }
            
            // Comparison operators
            if (op->token.type == "COMPARISON") {
                double a = toNumber(left);
                double b = toNumber(right);
                bool result = false;
                if (op->token.value == ">") result = a > b;
                else if (op->token.value == "<") result = a < b;
                else if (op->token.value == "==") result = a == b;
                else if (op->token.value == "!=") result = a != b;
                return make_shared<ASTNode>(Token("BOOL", result ? "true" : "false"));
            }
        }
        
        // INPUT statement (i/p)
        if (node->isList && node->children.size() == 2 && node->children[0]->token.type == "INPUT") {
            string varName = node->children[1]->token.value;
            string inputValue;
            cout << "  [INPUT] Enter value for " << varName << ": ";
            getline(cin, inputValue);
            
            Token valueToken;
            if (isNumeric(inputValue)) {
                if (inputValue.find('.') != string::npos) {
                    valueToken = Token("FLOAT", inputValue);
                } else {
                    valueToken = Token("INT", inputValue);
                }
            } else {
                valueToken = Token("STRING", inputValue);
            }
            
            if (symbols_.exists(varName)) {
                symbols_.write(varName, valueToken);
            } else {
                symbols_.declare(varName, valueToken, 0);
            }
            return make_shared<ASTNode>(valueToken);
        }
        
        // OUTPUT statement (o/p)
        if (node->isList && node->children.size() == 2 && node->children[0]->token.type == "OUTPUT") {
            ASTNodePtr value = eval(node->children[1]);
            cout << value->token.value << endl;
            return value;
        }
        
        // IF statement
        if (node->isList && node->children.size() >= 2 && node->children[0]->token.type == "IF") {
            bool condition = toBool(eval(node->children[1]));
            if (condition) {
                return eval(node->children[2]);
            }
            for (size_t i = 3; i < node->children.size(); i++) {
                auto& child = node->children[i];
                if (child->isList && child->children.size() >= 2) {
                    if (child->children[0]->token.type == "ELIF" && toBool(eval(child->children[1]))) {
                        return eval(child->children[2]);
                    }
                    if (child->children[0]->token.type == "ELSE") {
                        return eval(child->children[1]);
                    }
                }
            }
            return nullptr;
        }
        
        // WHILE loop
        if (node->isList && node->children.size() >= 2 && node->children[0]->token.type == "WHILE") {
            int iterations = 0;
            while (toBool(eval(node->children[1]))) {
                if (node->children.size() > 2) eval(node->children[2]);
                iterations++;
                if (iterations > 1000) break;
            }
            return nullptr;
        }
        
        // END statement
        if (node->token.type == "END") {
            return node;
        }
        
        // Variable reference (print value)
        if (node->token.type == "VARIABLE") {
            Token val = symbols_.read(node->token.value);
            cout << val.value << endl;
            return make_shared<ASTNode>(val);
        }
        
        // String literal
        if (node->token.type == "STRING") {
            return node;
        }
        
        // Block (list of statements)
        if (node->isList && !node->children.empty()) {
            ASTNodePtr result = nullptr;
            for (auto& stmt : node->children) {
                result = eval(stmt);
            }
            return result;
        }
        
        return node;
    }
};