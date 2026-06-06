#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Token.h"
// SymbolTable class: Program ke saare variables ko track karta hai
// Har variable ka naam, type, value, aur declaration line store karta hai
class SymbolTable {
private:
    // Symbol struct: Ek variable ki poori information store karta hai
    struct Symbol {
        Token value;         // Variable ki current value (Token form mein)
        int declaredAt;      // Kis line number par declare hua tha
        bool initialized;    // Kya variable ko value assign ho chuki hai
        std::string type;    // Variable ka type (INT, FLOAT, STRING, BOOL)
    };
    
    std::unordered_map<std::string, Symbol> table;  // Naam se Symbol dhundhne ke liye hash map
    std::vector<std::string> declarationOrder;      // Declaration order maintain karne ke liye (print ke liye)
    
public:
    // declare(): Naya variable declare karta hai
    // Agar variable already exist karta hai to update kar deta hai
    void declare(const std::string& name, const Token& value, int line) {
        if (table.find(name) != table.end()) {
            // Variable already exists - update karo
            table[name].value = value;
            table[name].initialized = true;
            table[name].type = value.type;
            std::cout << "  [SYMBOL] Updated " << name << " = " << value.value << std::endl;
            return;
        }
   
        // Naya variable add karo
        table[name] = {value, line, true, value.type};
        declarationOrder.push_back(name);  // Order mein bhi add karo
        std::cout << "  [SYMBOL] Declared " << name << " = " << value.value << std::endl;
    }
    // write(): Existing variable ki value update karta hai
    // Agar variable declare nahi hua to error throw karta hai
    void write(const std::string& name, const Token& value) {
        if (table.find(name) == table.end()) {
            throw std::runtime_error("Undeclared variable: " + name);
        }
        table[name].value = value;
        table[name].initialized = true;
        table[name].type = value.type;
        std::cout << "  [SYMBOL] Wrote " << name << " = " << value.value << std::endl;
    }
    // read(): Variable ki current value return karta hai
    // Agar variable declare nahi hua to error throw karta hai
    Token read(const std::string& name) const {
        auto it = table.find(name);
        if (it == table.end()) {
            throw std::runtime_error("Undeclared variable: " + name);
        }
        return it->second.value;
    }
    // exists(): Check karta hai ke variable declare hua hai ya nahi
    bool exists(const std::string& name) const {
        return table.find(name) != table.end();
    }
    // printSymbolTable(): Poori symbol table ko formatted table ki tarah print karta hai
    // Debugging aur verification ke liye useful hai
    void printSymbolTable() const {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "SYMBOL TABLE" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << std::left << std::setw(15) << "Name" 
                  << std::setw(10) << "Type" 
                  << std::setw(15) << "Value" 
                  << "Line" << std::endl;
        std::cout << std::string(60, '-') << std::endl;   
        // Declaration order mein print karo (readable output ke liye)
        for (const auto& name : declarationOrder) {
            auto it = table.find(name);
            if (it != table.end()) {
                std::cout << std::left << std::setw(15) << name
                          << std::setw(10) << it->second.type
                          << std::setw(15) << it->second.value.value
                          << it->second.declaredAt << std::endl;
            }
        }
        std::cout << std::string(60, '=') << "\n" << std::endl;
    }  
    // printAllValues(): Sirf final variable values ko print karta hai
    // Program execution ke end mein output dikhane ke liye
    void printAllValues() const {
        std::cout << "\nFINAL VARIABLE VALUES:" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        for (const auto& name : declarationOrder) {
            auto it = table.find(name);
            if (it != table.end()) {
                std::cout << name << " = " << it->second.value.value << std::endl;
            }
        }
        std::cout << std::string(40, '-') << std::endl;
    }
};