
# Palladium Language Development Project  

Welcome to the **Palladium Language Development Project**, a collaborative effort to learn and build a programming language from scratch.
Our goal is to explore every aspect of language design, from lexing and parsing to virtual machine execution and memory management.  

This project is perfect for anyone who wants to dive deep into the internals of programming languages, virtual machines, and compilers.  

---

## Table of Contents  

- [What is Palladium?](#what-is-palladium)  
- [Current Features](#current-features)  
- [Getting Started](#getting-started)  
- [How to Contribute](#how-to-contribute)  
- [Future Goals](#future-goals)  
- [Contact and Collaboration](#contact-and-collaboration)  

---

## What is Palladium?  

Palladium is not just a programming language—it’s a learning journey. By collaborating on Palladium, you'll gain hands-on experience with:  

- Designing a lexer and parser.  
- Implementing a virtual machine (VM).  
- Managing memory allocation with advanced techniques.  
- Exploring new language features and extending syntax.  

---

## Current Features  

### Lexer  

- A fully functional lexer that can tokenize source code with support for arbitrary token lookahead.  
- Easy to extend with new tokens and grammar rules.  

### Virtual Machine  

- A stack-based virtual machine capable of executing custom instructions.  
- Segmented memory management, allowing dynamic allocation of various block sizes.  
- Native function integration for extending the language with external libraries.  

### Testing  

- Comprehensive unit tests for memory management and instruction execution.  

---

## Getting Started  

### Prerequisites  

- A C++20-compatible compiler (e.g., GCC, Clang).  
- CMake (version 3.16 or higher).  

### Building the Project  

```bash  
git clone https://github.com/yourusername/palladium  
cd palladium   
cmake .  
make  
make test 


