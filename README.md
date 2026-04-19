# 🧩 Multithreaded Sudoku Solver

A C++ program that solves multiple Sudoku puzzles simultaneously using multithreading. It reads puzzles from an input file (input_sudoku.txt), distributes them across threads, solves each using a backtracking algorithm, and writes the results to an output file.

## ⚙️ How It Works

1. The program reads the number of available CPU cores and spawns that many threads
2. Each thread pulls an unsolved puzzle from the input file (protected by a mutex)
3. The puzzle is solved using a **recursive backtracking algorithm**
4. The solved puzzle is written to the output file (protected by a separate mutex)
5. Threads keep pulling puzzles until the input file is exhausted

This approach allows multiple puzzles to be solved in parallel, taking full advantage of multi-core processors.

## 🔒 Thread Safety

Two mutexes protect shared resources:
- `inFileMutex` — guards reading from the input file
- `outFileMutex` — guards writing to the output file

## 🧠 Solving Algorithm

The solver uses **recursive backtracking**:
- Scans the grid left-to-right, top-to-bottom for empty cells (`0`)
- Tries placing digits 1–9 in each empty cell
- Validates the candidate against its row, column, and 3×3 subgrid
- Recurses to the next cell; backtracks if no valid digit exists

## 📁 File Format

### Input (`input_sudoku.txt`)
Each puzzle starts with a grid name, followed by 9 rows of 9 digits. `0` represents an empty cell.
```
Grid 01
003020600
900305001
001806400
...
```

### Output (`Lab3Prob1.txt`)
Each solved puzzle starts with its grid name, followed by 9 rows of space-separated digits.
```
Grid 01
4 8 3 9 2 1 6 5 7
9 6 7 3 4 5 8 2 1
...
```

## 🚀 Build & Run

### Compile
```bash
g++ -std=c++11 -o sudoku_solver sudokuSolver.cpp -lpthread
```

### Run
```bash
./sudoku_solver input_sudoku.txt
```

The solved puzzles will be written to `Lab3Prob1.txt` in the current directory.

### Windows (Visual Studio)
Build the solution in Visual Studio, then run:
```
sudokuSolver.exe input_sudoku.txt
```

## 📂 Project Structure

```
sudoku-solver/
├── sudokuSolver.cpp         # Main source file
└── input_sudoku.txt         # Sample input with 50 puzzles
```

## 🛠️ Dependencies

- C++11 or later
- Standard library only (`<thread>`, `<mutex>`, `<fstream>`)
- No external libraries required
