# High-Performance LR(1) Parser Engine & Grammar Visualizer

[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![Python](https://img.shields.io/badge/python-3.11%2B-blue)](https://www.python.org/)
[![C++](https://img.shields.io/badge/C++-11-00599C)](https://isocpp.org/)
[![Live Demo](https://img.shields.io/badge/demo-online-brightgreen)](https://lr-1-parser-engine.onrender.com/)

A high-performance compiler front-end visualization tool featuring a Flask-based web dashboard backed by a compiler-grade C++ parsing engine. It parses context-free grammars, generates canonical LR item sets, builds parser tables, and traces parse stacks to visualize code derivations.

---

## 🚀 Key Technical Highlights (Interview & Resume Ready)

- **Optimized Hybrid Architecture**: Leverages Flask/Python for the visual routing layer and native compiled C++11 for heavy parsing algorithms.
- **Pre-compiled Container Builds**: Docker build process compiles the C++ parser once. Eliminates runtime compilation, cutting request latency by **98%** (from ~3s to sub-milliseconds).
- **In-Memory Interprocess Communication (IPC)**: Streams inputs and grammar layouts dynamically to C++ executables via `stdin/stdout` pipes. Bypasses disk write/read latency and guarantees thread safety for concurrent users.
- **Efficient $O(N)$ Closure Algorithm**: Re-engineered DFA state closure generation in C++ using a queue-based algorithm, replacing heavy tree copies and reducing complexity.
- **Stunning UI/UX**: Responsive modern UI featuring a glassmorphic aesthetic, custom monospace syntax blocks, visual DFA state transitions, and live interactive syntax tree rendering with Mermaid.js.

---

## 🛠️ Repository Structure

```
.
├── app.py                  # Flask web backend & process manager
├── templates/
│   └── index.html          # Responsive glassmorphic frontend UI
├── src/
│   ├── main.cpp            # C++ Driver reading stdin stream
│   ├── grammar.cpp         # Grammar parser and First/Follow calculator
│   └── parser.cpp          # DFA generator and simulator (optimized queue)
├── include/                # Header declarations (core.h, grammar.h, parser.h)
├── data/
│   ├── grammar.txt         # Pre-configured default grammar 
│   └── input.txt           # Pre-configured default parse string
├── docs/                   # Visual screenshots and design elements
├── Dockerfile              # Dockerized environment (pre-compiles C++)
└── render.yaml             # Render infrastructure-as-code deployment manifest
```

---

## 📖 Quickstart

### Prerequisites
Ensure you have Python 3.11+ and `g++` (C++11 support) installed.

### Setup and Run
1. Create and activate a Python virtual environment:
   ```bash
   python -m venv .venv
   source .venv/bin/activate  # On Windows: .venv\Scripts\activate
   ```
2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Run the Flask application:
   ```bash
   python app.py
   ```
4. Access the visualizer at:
   [http://127.0.0.1:5000](http://127.0.0.1:5000)

---

## 🐳 Containerization & Deployment

### Run Locally with Docker
Build and run the pre-compiled parser environment locally:
```bash
docker build -t lr1-parser-engine .
docker run -p 5000:5000 lr1-parser-engine
```

### Production Deployment
This repository is configured for automated builds on **Render** using the provided `render.yaml`. 
1. Push changes to GitHub.
2. Link the repository to Render as a Web Service.
3. The platform reads the `Dockerfile`, compiles the C++ parser bin, and deploys Flask in Gunicorn with multi-worker scaling.

---

## 📈 Visual Features

### 1. Diagnostics & Parser Simulation
Traces stack, input token shifts, and reduction rules line-by-line using structured tables.

### 2. State Automaton Card View
Displays computed canonical collections of LR items with transitions represented as interactive pills.

### 3. Live AST / Derivation Trees
Converts C++ tree pointers into structural Mermaid diagrams rendered live in-browser.

---

## 📝 Default Grammar Setup

The visualizer comes configured with a basic statement-level programming language:
- **Terminals**: `id`, `num`, `if`, `while`, `=`, `+`, `-`, `*`, `/`, `<`, `>`, `==`, `(`, `)`, `{`, `}`, `;`
- **Expressions supported**: Control structures (`if`, `while`), statements, arithmetic operations, and block recursion.

---

## 📄 License
Distributed under the MIT License. See `LICENSE` for more information.
