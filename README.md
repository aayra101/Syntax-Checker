# DSA Syntax Checker — C Static Code Analyzer

> A **DSA Minor Project** submitted to **Manipal University Jaipur**  
> by **Aayra Gupta** | B.Tech CCE-'B' | Roll No: 2428010132  
> Under the guidance of **Dr. Amit Sharma**, Department of CCE

---

## 📌 Problem Statement

A **Static Code Analyzer** that reads a C program and performs various analytical checks **without executing the code**. Designed to help beginner programmers identify coding errors, understand code structure, and improve code quality.

---

## ✅ Features

| Feature | Description |
|---|---|
| 🧹 Comment Removal | Strips `//` comments for clean parsing |
| 🔑 Keyword Detection | Identifies & counts C keywords with line numbers |
| 📦 Variable Detection | Detects declared variables by type |
| ⚙️ Function Detection | Distinguishes user-defined vs built-in functions |
| 🔲 Bracket Validation | Validates `{}`, `()`, `[]` balance |
| 💾 Memory Estimation | Estimates memory used by declared variables |
| ✏️ Syntax Validation | Checks `printf`, `scanf`, `for`, `while` statements |
| 📊 Summary Report | Outputs a complete analysis report to `output.txt` |

---

## 🛠️ Tech Stack

- **Language**: C
- **Concepts Used**: File Handling, Strings, Arrays, Loops, Functions, Stack Logic (bracket validation)
- **Compiler**: GCC

---

## 📁 Project Structure

```
DSA-Syntax-Checker/
│
├── main.c          # Full source code of the analyzer
├── input.txt       # Sample C program to analyze
├── output.txt      # Generated analysis report (auto-created on run)
└── README.md       # Project documentation
```

---

## 🚀 How to Run

### Step 1 — Compile
```bash
gcc main.c -o syntax_checker
```

### Step 2 — Run
```bash
./syntax_checker
```
On Windows:
```bash
syntax_checker.exe
```

### Step 3 — Enter Input File
```
Enter input file path (e.g. input.txt): input.txt
```

### Step 4 — View Output
The analysis report is saved to `output.txt` automatically.

---

## 📊 Sample Output

```
======================================
   DSA SYNTAX CHECKER - OUTPUT REPORT
   File Analyzed: input.txt
======================================

----- BRACKET / PARENTHESIS CHECK -----
  [OK]      Curly braces balanced    { 6  } 6
  [WARNING] Unbalanced parentheses   ( 9  vs  ) 8

----- KEYWORDS WITH LINE NUMBER -----
  Line 1 [1]: include
  Line 2 [2]: int
  ...

----- VARIABLES WITH COUNTS -----
  Total int   : 2
  Total float : 1
  Total char  : 3

----- MEMORY CONSUMED BY VARIABLES -----
  int   : 8 bytes
  float : 4 bytes
  char  : 3 bytes
  TOTAL MEMORY USED: 15 bytes

----- SYNTAX CHECK -----
  [ERROR] Line 5: Missing semicolon --> printf("hd")
  [ERROR] Line 10: for loop may be malformed

====== SUMMARY ======
  Total Lines     : 28
  Keywords Found  : 17
  Variables Found : 6
  Functions Found : 3
  Built-ins Used  : 4
====================
```

---

## 🧠 DSA Concepts Used

- **File Handling & String Manipulation** — Read source file line by line, tokenize and process
- **Arrays & Strings** — Store code lines, variable names, keywords, and built-in function lists
- **Loops & Conditional Statements** — Repetitive checks for syntax validation and pattern matching
- **Counters & Iterative Algorithms** — Count brackets, keywords, and syntax elements
- **Stack Logic (Conceptual)** — Balanced parentheses, curly braces, and square brackets validation
- **Functions** — Modular design with separate functions for each analysis step

---

## 📋 Algorithm (High-Level)

```
1. Open input C file
2. Read line by line → remove comments → skip empty lines
3. Store cleaned lines with original line numbers
4. For each line:
   → Count brackets { } ( ) [ ]
   → Extract keywords
   → Detect built-in functions
   → Detect user-defined functions
   → Identify variable declarations
   → Estimate memory usage
   → Perform syntax checks
5. Write full report to output.txt
6. Display summary in terminal
```

---

## 👩‍💻 Author

**Aayra Gupta**  
B.Tech CCE-'B', Manipal University Jaipur  
Roll No: 2428010132  
GitHub: [@Aayra06](https://github.com/Aayra06)

⭐ Star this repo if it helped you understand DSA concepts!

