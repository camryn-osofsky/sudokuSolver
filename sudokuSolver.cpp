/*
* Author: Camryn Osofsky
* 
* Description:
* Takes in a path to an input sudoku puzzle and uses multithreading to solve the puzzle.
*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>
#include <vector>

#define N 9

using namespace std;

// global variable declarations
mutex outFileMutex;
mutex inFileMutex;
fstream outFile;
fstream inFile;

// class to define SudokuGrid objects
class SudokuGrid {
    // public variables and functions
    public:
        string m_strGridName;
        unsigned char gridElement[9][9];

        friend fstream& operator>>(fstream& is, SudokuGrid& gridIn);
        friend fstream& operator<<(fstream& os, const SudokuGrid& gridIn);
        bool solve();

    // private helper method declaration
    private:
        bool isValid(int row, int col, int num);
        bool solveHelper(int row, int col);

};

// function to override >> operator
// >> operator is used to read in SudokuGrid objects
fstream& operator>>(fstream& is, SudokuGrid& gridIn) {
    string tempString;
    char buffer[2] = {'\0', '\0'};

    getline(is, gridIn.m_strGridName);
    for (int i = 0; i < N; ++i) {
        getline(is, tempString);

        for (int j = 0; j < N; ++j) {
            buffer[0] = tempString[j];
            gridIn.gridElement[i][j] = atoi(buffer);
        }
    }
    return is;
}

// function to override << operator
// << operator is used to write solved grid to the output file
fstream& operator<<(fstream& os, const SudokuGrid& gridIn) {
    os << gridIn.m_strGridName << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            os << static_cast<int>(gridIn.gridElement[i][j]) << " ";
        }
        os << endl;
    }
    return os;
}

// checks if the number can be placed in the spot
bool SudokuGrid::isValid(int row, int col, int num) {
    for (unsigned int x = 0; x < N; x++) {
        if (gridElement[row][x] == num || gridElement[x][col] == num) {
            return false;
        }

        unsigned int startR = row - (row % 3);
        unsigned int startC = col - (col % 3);

        for (unsigned int i = 0; i < 3; i++) {
            for (unsigned int j = 0; j < 3; j++) {
                if (gridElement[i + startR][j + startC] == num) {
                    return false;
                }
            }
        }
    }

    return true;
}

// helper method for solve function
bool SudokuGrid::solveHelper(int row, int col) {
    if (row == 8 && col == 9) {
        return true;
    }
    if (col == 9) {
        row++;
        col = 0;
    }

    // if the current element is already occupied by a number other than 0, move to the next element
    if (gridElement[row][col] != 0) {
        return solveHelper(row, col + 1);
    }

    for (int num = 1; num <= 9; num++) { //valid numbers are 1 - 9
        if (isValid(row, col, num)) { //check validation, if yes, put the number in the grid
            gridElement[row][col] = num;
            if (solveHelper(row, col + 1)) {//recursively go for other rooms in the grid
                return true;
            }
            gridElement[row][col] = 0; //turn to unassigned space when conditions are not satisfied
        }
    }
    return false;
}

// calls the helper method beginning at the top left corner of the grid
bool SudokuGrid::solve() {
    return solveHelper(0, 0);
}

// uses mutexes to help to solve the puzzles simultaneously
void solveSudokuPuzzles() {
    do {
        SudokuGrid grid;

        {
            lock_guard<mutex> guard(inFileMutex);
            if (inFile.eof()) 
            {
                break;
            }
            else {
                inFile >> grid;
            }
        }

        grid.solve();

        {
            lock_guard<mutex> guard(outFileMutex);
            outFile << grid;
        }

    } while (true);
}

// main function used to open files, create threads, and have threads solve the puzzles
int main(int argc, char* argv[]) {
    // names of the input and output files
    string inputFile = argv[1];
    string outputFile = "output_sudoku.txt";

    // open the files
    inFile.open(inputFile, ios::in);
    outFile.open(outputFile, ios::out);

    // determine the number of threads
    unsigned int numThreads = thread::hardware_concurrency();

   vector<thread> threads;

   for (unsigned int i = 0; i < numThreads - 1; i++)
   {
       threads.push_back(thread(solveSudokuPuzzles));
   }

   // let the main thread handle the rest of the grids
   solveSudokuPuzzles();

   // wait for all threads to finish
   for (unsigned int i = 0; i < numThreads - 1; i++) 
   {
       threads[i].join();
   }

   // close the files
    inFile.close();
    outFile.close();

    return 0;
}
