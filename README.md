# W++ Programming Language

> **W++ (W Plus Plus)** is a custom interpreted programming language built entirely from scratch in C++. It combines English programming concepts with Urdu-inspired keywords, creating an accessible and educational programming experience for Urdu-speaking developers and learners.

---

## 📖 Introduction

W++ (pronounced **"W Plus Plus"**) is a lightweight interpreted programming language designed to demonstrate the complete language execution pipeline:

1. **Lexical Analysis (Lexer)**
2. **Syntax Analysis (Parser)**
3. **Interpretation (Interpreter)**

The language was developed from scratch using modern C++ and serves as both an educational compiler-construction project and a functional programming language for small-scale applications.

The name **W++** reflects its local ("desi") identity while maintaining familiarity with traditional programming languages through the `++` suffix.

---

## 🎯 Objectives

* Build a complete interpreter pipeline from scratch in C++.
* Create a programming language using Urdu-inspired keywords.
* Demonstrate lexical analysis, parsing, and interpretation.
* Provide a clean single-file execution model.
* Support variables, arithmetic expressions, conditionals, and loops.
* Help students understand how programming languages work internally.

---

## ⚙️ Requirements

* C++11 or later
* Any modern C++ compiler:

  * GCC
  * Clang
  * MSVC (Visual Studio)

---

## 🏗️ Project Architecture

```text
Source Code
     │
     ▼
 ┌─────────┐
 │ Lexer   │
 └─────────┘
     │
     ▼
 ┌─────────┐
 │ Parser  │
 └─────────┘
     │
     ▼
 ┌─────────┐
 │ AST     │
 └─────────┘
     │
     ▼
 ┌─────────────┐
 │ Interpreter │
 └─────────────┘
     │
     ▼
 Program Output
```

---

## ✨ Features

### Complete Interpreter Pipeline

* Lexer converts source code into typed tokens.
* Parser builds an Abstract Syntax Tree (AST).
* Interpreter executes the AST.
* Fully implemented from scratch in C++.
* No external libraries or frameworks required.

---

## 🌍 Urdu-Inspired Keywords

W++ introduces Urdu-inspired keywords to make programming more intuitive for Urdu-speaking users.

Example:

```w++
rakho x = 10;

agar (x > 5) {
    dikhao("Number is greater than 5");
}
```

This unique approach combines programming concepts with local language familiarity.

---

## 📊 Data Type Support

### Integers

```w++
rakho x = 10;
```

### Floating Point Numbers

```w++
rakho pi = 3.14;
```

### Strings

```w++
rakho name = "Ali";
```

### Booleans

```w++
rakho result = x > 5;
```

### Dynamic Typing

Types are automatically inferred during assignment.

---

## ➕ Arithmetic Operations

Supported arithmetic operators:

| Operator | Description    |
| -------- | -------------- |
| +        | Addition       |
| -        | Subtraction    |
| *        | Multiplication |
| /        | Division       |
| ()       | Parentheses    |

Example:

```w++
rakho result = (10 + 5) * 2;
dikhao(result);
```

---

## 🔍 Comparison Operators

| Operator | Meaning                  |
| -------- | ------------------------ |
| ==       | Equal To                 |
| !=       | Not Equal To             |
| >        | Greater Than             |
| <        | Less Than                |
| >=       | Greater Than Or Equal To |
| <=       | Less Than Or Equal To    |

Example:

```w++
agar (age >= 18) {
    dikhao("Adult");
}
```

---

## 🧠 Core Components

### Lexer

Responsible for:

* Reading source code
* Identifying keywords
* Detecting identifiers
* Parsing literals
* Generating tokens

Example:

```text
rakho x = 10;
```

Produces:

```text
KEYWORD(rakho)
IDENTIFIER(x)
ASSIGN(=)
INTEGER(10)
SEMICOLON(;)
```

---

### Parser

Responsible for:

* Consuming tokens
* Validating syntax
* Constructing the AST

Example:

```text
AssignmentNode
 ├── Variable: x
 └── Value: 10
```

---

### Abstract Syntax Tree (AST)

Represents the structure of the program.

Example:

```text
BinaryExpr
 ├── Number(5)
 ├── +
 └── Number(3)
```

---

### Interpreter

Responsible for:

* Traversing the AST
* Evaluating expressions
* Managing variables
* Producing final output

---

## 📂 Project Structure

```text
W++/
│
├── AST.h
├── Data.h
├── Helpers.h
├── Token.h
├── SymbolTable.h
├── Parser.cpp
├── Interpreter.cpp
│
└── main.cpp
```

### File Description

| File            | Purpose                              |
| --------------- | ------------------------------------ |
| AST.h           | AST node definitions                 |
| Token.h         | Token structure and token types      |
| Data.h          | Runtime value representation         |
| Helpers.h       | Utility/helper functions             |
| SymbolTable.h   | Variable storage and lookup          |
| Parser.cpp      | Syntax analysis and AST construction |
| Interpreter.cpp | AST execution engine                 |
| main.cpp        | Program entry point                  |

---

## 🚀 Compilation

Using GCC:

```bash
g++ -std=c++11 *.cpp -o wpp
```

Run:

```bash
./wpp
```

Windows (MSVC):

```bash
cl /EHsc *.cpp
```

---

## 📝 Example Program

```w++
rakho x = 10;
rakho y = 20;

rakho sum = x + y;

dikhao(sum);
```

Output:

```text
30
```

---

## 🔄 Execution Flow

```text
Source Code
      │
      ▼
Lexer
      │
      ▼
Tokens
      │
      ▼
Parser
      │
      ▼
AST
      │
      ▼
Interpreter
      │
      ▼
Output
```

---

## 🎓 Educational Value

This project helps students understand:

* Compiler Design Fundamentals
* Lexical Analysis
* Parsing Techniques
* Abstract Syntax Trees
* Expression Evaluation
* Symbol Tables
* Interpreter Architecture
* Programming Language Design

---

## 📸 Sample Output

```text
Enter W++ Program

rakho x = 5;
rakho y = 10;

dikhao(x + y);

Output:
15
```

---

## 🔮 Future Improvements

Possible enhancements include:

* Functions
* Arrays
* Classes and Objects
* Modules
* File Handling
* Error Recovery
* Standard Library
* User Input Support
* Better Debugging Tools

---

## ✅ Conclusion

W++ is a complete interpreted programming language implemented entirely in C++. The project successfully demonstrates all major phases of language execution: lexical analysis, syntactic parsing, and semantic interpretation.

By combining traditional programming concepts with Urdu-inspired keywords, W++ provides a culturally familiar and educational environment for learners while showcasing the internal workings of programming language implementation.

This project serves as both a practical interpreter and a valuable learning resource for students interested in compiler construction, language design, and systems programming.
