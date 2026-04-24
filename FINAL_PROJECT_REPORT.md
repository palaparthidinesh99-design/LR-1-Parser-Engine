# Final Project Report: LR(1) Parser Engine & Visualization Dashboard

## 1. Project Abstract
The **LR(1) Parser Engine** is a comprehensive software tool designed to facilitate the study and practical application of compiler design principles. It implements a robust Bottom-Up parsing algorithm (LR(1)/SLR) with a modern web-based interface for real-time visualization of DFA states, parsing tables, and derivation trees.

---

## 2. Objectives
*   **Algorithmic Implementation:** To develop a C++ engine capable of computing FIRST/FOLLOW sets and constructing Canonical Collections of Items.
*   **Table Generation:** To automate the creation of ACTION and GOTO tables with built-in conflict resolution.
*   **Visual Debugging:** To provide an interactive dashboard that translates complex data structures into human-readable diagrams and tables.
*   **User Flexibility:** To allow users to define custom grammars with complex precedence and associativity rules.

---

## 3. Technical Architecture

### 3.1 Backend Engine (C++)
The core logic is written in C++ for maximum efficiency and algorithmic transparency.
*   **GrammarManager:** Handles rule tokenization and set calculations.
*   **Parser Class:** Manages the DFA (Deterministic Finite Automaton) state transitions and table populating.
*   **Tree Builder:** Constructs an Abstract Syntax Tree (AST) using a bottom-up approach during input reduction.

### 3.2 Middleware (Python/Flask)
The Flask server acts as a bridge, handling the compilation of the C++ source and managing the data flow between the engine and the UI. It ensures that the engine is always synchronized with the user's latest grammar modifications.

### 3.3 Frontend Dashboard (HTML/JS/CSS)
*   **Glassmorphism UI:** A sleek, modern interface utilizing translucent panels and high-contrast typography.
*   **Mermaid.js:** An integration that renders post-fix tree data into interactive SVG diagrams.
*   **Responsive Tables:** Implemented custom horizontal and vertical sticky behaviors to handle large-scale grammars (up to 200+ states).

---

## 4. Key Features & Innovations

### 4.1 Advanced Conflict Resolution
The system supports a `PRECEDENCE` section in the grammar file. This allows it to resolve Shift/Reduce conflicts automatically—a feature typically found in production-grade tools like Yacc or Bison.

### 4.2 Robust Data Handling
To prevent terminal symbol conflicts (e.g., using `||` or `|` in the grammar), the system uses a **Tab-Separated Value (TSV)** communication protocol between the C++ engine and the JS frontend. This ensures 100% accurate column alignment regardless of the symbols used.

### 4.3 Aligned Derivation Trees
The tree visualization implements an "Invisible Sink Node" logic. This forces all terminal leaf nodes to align horizontally at the bottom of the graph, making the resulting "Input String" clearly visible in a single line.

---

## 5. System Execution Flow
1.  **Input:** User provides a Context-Free Grammar (CFG) and a test string.
2.  **Analysis:** The C++ engine augments the grammar and computes FIRST/FOLLOW sets.
3.  **DFA Construction:** The engine builds a collection of state items using Closure and Goto operations.
4.  **Parsing:** The engine simulates the stack-based parsing process.
5.  **Output:** The engine exports the DFA, Tables, and AST to the dashboard for visualization.

---

## 6. Conclusion
The **LR(1) Parser Engine** successfully demonstrates the integration of classical compiler theory with modern web visualization techniques. It serves as a powerful educational tool, making the complex "black box" of LR parsing transparent and accessible to students and developers alike.

---

**Author:** [Your Name]  
**Subject:** Compiler Design (Sem 4)  
**Repository:** [https://github.com/palaparthidinesh99-design/LR-1-Parser-Engine](https://github.com/palaparthidinesh99-design/LR-1-Parser-Engine)
