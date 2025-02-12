TURING LANGUAGE INTERPRETER
==========================

⚠️ EXPERIMENTAL PROJECT - NOT FOR PRODUCTION USE ⚠️
-------------------------------------------------

About
-----
A simple programming language interpreter written in C.
This is a learning project to understand how compilers and interpreters work.

Features
--------
✓ Simple and readable syntax
✓ Basic data types (numbers and strings)
✓ Built-in print() function
✓ Basic math operations
✓ Variables and assignments
✓ Clear error messages
✓ Fast execution for simple programs

Limitations
-----------
⚠️ NOT FOR PRODUCTION USE
⚠️ No garbage collection
⚠️ Very limited features
⚠️ No optimizations
⚠️ No modern programming features
⚠️ May contain bugs and security issues

Quick Start
-----------
1. Build:
   make clean
   make

2. Run:
   ./turing yourfile.tr

Example Code
-----------
function main() {
    print("Hello, World!");
    let x = 42;
    print(x);
}

Technical Details
---------------
- Written in C
- No external dependencies
- Simple lexer and parser
- Direct execution (no bytecode)
- Basic error handling

Development Status
----------------
This is an EXPERIMENTAL project meant for:
✓ Learning compiler basics
✓ Understanding interpreters
✓ Experimenting with language design
✓ Educational purposes

NOT meant for:
✗ Production use
✗ Serious applications
✗ Commercial use without extensive modifications
✗ Processing sensitive data

Testing
-------
Run tests with:
make test

License
-------
MIT License - See LICENSE file for details

Important Notice
--------------
This project is a PROOF OF CONCEPT and should not be used in any production environment.
Contributors should understand this is a learning project with no guarantees of security or stability.

Contact
-------
For questions or feedback, please open an issue in the repository.

REMEMBER: USE AT YOUR OWN RISK
----------------------------- 