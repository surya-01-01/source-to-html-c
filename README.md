# 🖍️ Source to HTML Syntax Highlighter (C Code Parser)

This project implements a **syntax highlighter** that parses C source files and generates an equivalent **HTML file** with syntax highlighting. The goal is to display readable, color-coded C code in a web browser—similar to how modern IDEs and online code viewers work.

---

## 🎯 Project Objective

To simulate the functionality of syntax highlighting editors (like VS Code, Sublime, Notepad++) by writing a **C program** that:
- Parses `.c` source code files
- Identifies tokens like keywords, strings, comments, numbers, etc.
- Wraps them with appropriate HTML tags and CSS classes
- Generates a final `.html` file that can be viewed in a browser

---

## 🔍 Why Syntax Highlighting?

Syntax highlighting:
- Enhances code **readability and maintainability**
- Helps spot **syntax errors** quickly (e.g., missing quotes or braces)
- Improves **learning and documentation clarity** for readers
- Makes large codebases easier to scan and debug visually

---

## 🧰 Technologies Used

- **Language:** C
- **Concepts:** Finite State Machine (FSM), Tokenization, File I/O, HTML generation
- **Output:** HTML + CSS for browser rendering

---

## 🧠 Supported Token Types

The parser highlights the following syntax elements:
- `Keywords` (e.g., `int`, `for`, `return`)
- `Preprocessor directives` (e.g., `#include`, `#define`)
- `String literals` (e.g., `"hello"`)
- `Character constants` (e.g., `'a'`)
- `Numerical constants` (e.g., `42`, `3.14`)
- `Comments` (single-line `//` and multi-line `/* */`)

---



