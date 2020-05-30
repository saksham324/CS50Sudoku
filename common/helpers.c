/*
 * helpers.c - implements methods from helpers.h relating to sudoku_t
 * Author: Team Rocket
 * CS50, Spring 2020
 * A board is a 9x9 2D array that holds its size
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "helpers.h"
#include "board.h"
#include "counters.h"

static counters_t *getRow(sudoku_t *b, int r);
static counters_t *getColumn(sudoku_t *b, int col);
static int generateRandomNum(counters_t *row, counters_t *column, counters_t *cell);

/*********** prototypes **************/
static int getRandNumber(int min, int max);

bool testSolver(sudoku_t *b);
bool testSolverHelper(sudoku_t *b, int pos);
bool isNumberPresent(sudoku_t *b, int r, int c, int v);

bool checkRow(sudoku_t *b, int r, int c, int v);
bool checkCol(sudoku_t *b, int r, int c, int v);
bool checkGrid(sudoku_t *b, int r, int c, int v);

int findArrayRow(sudoku_t *b, int pos);
int findArrayCol(sudoku_t *b, int pos);

/************ generateRandomGrid ************/
/*
 * Populates a specified 3x3 grid within the board with numbers 1-9
 * 
 * Caller provides:
 *  A valid sudoku board
 * We guarantee:
 *  Each number is used only once
 * Caller is responsible for:
 *  Nothing
 */
void generateRandomGrid(sudoku_t *b, int rStart, int cStart) {
  if (!b || rStart < 0 || cStart < 0 || b->dimension < rStart + 3 || b->dimension < cStart + 3) return;
  counters_t *grid = counters_new();

  // Iterate through each row of the 3x3
  for (int i = rStart; i < rStart + 3; i++) {
    counters_t *row = getRow(b, rStart);

    // Iterate through each column of the 3x3
    for (int j = cStart; j < cStart + 3; j++) {
      counters_t *column = getColumn(b, cStart);
      int insert = generateRandomNum(row, column, grid);
      counters_add(grid, insert);
      b->board[i][j] = insert;
      counters_delete(column);
      } 
      
    counters_delete(row);
  }

  counters_delete(grid);
  return;
}

static counters_t *getRow(sudoku_t *b, int r) {
  counters_t *row = counters_new();
  for (int i = 0; i < b->dimension; i++) {
    if (!(b->board[r][i] == 0)) {
      counters_add(row, b->board[r][i]);
    }
  }
  return row;
}

static counters_t *getColumn(sudoku_t *b, int col) {
  counters_t *column = counters_new();
  for (int i = 0; i < b->dimension; i++) {
    if (!(b->board[i][col] == 0)) {
      counters_add(column, b->board[i][col]);
    }
  }
  return column;
}

/********** generateRandomNum **********/
/* 
 * Generates a random number from 1-9 and checks whether that number has already been used 
 * in a given row, column, or grid
 * 
 * Caller provides: 
 *  a set of numbers in a row, column, and grid seen by one cell
 * We guarantee: 
 *  returns a number unique to a row, column, and grid seen by a cell
 * Caller is responsible for: 
 *  Nothing
 */
static int generateRandomNum(counters_t *row, counters_t *column, counters_t *grid) {
  int insert;
  while(1) {
    // Generates a random number between 1 and 9
    insert = (rand()%9) + 1;

    // Check if that number is in the current row, column, and cell
    if (counters_get(row, insert) == 0 && counters_get(column, insert) == 0 && counters_get(grid, insert) == 0) {
      return insert;
    }
  }
  return insert;
}
  

/************ populateBoard ************/
/*
 * Takes a sudoku board and fills it in completely using the solver functionality
 * 
 * Caller provides:
 *  A valid sudoku board
 * We guarantee:
 *  A board with one solution is returned
 * Caller is responsible for:
 *  Nothing
 */
bool populateBoard(sudoku_t *b) {
  if (!b) return false;
   
  return false;
}

/************ removeNumbers ************/
/*
 * Removes n numbers from a valid sudoku board and checks if board is still unique
 * 
 * Caller provides:
 *  A valid sudoku board and a number between 0 and n^2 - 17 numbers to remove
 * We guarantee:
 *  A board with one solution is created
 * We return:
 *  True if numbers removed successfully, false if any error
 * Caller is responsible for:
 *  Nothing
 */
bool removeNumbers(sudoku_t *b, int n) {
  if (!b || (((b->dimension) * (b->dimension)) - MIN_SPACES) < n) return false;
  // printf("N: [%i] < [%i]\n", ((b->dimension) * (b->dimension)) - MIN_SPACES, n);

  int numRemoved = 0;
  while (numRemoved < n) {
    int dim1 = getRandNumber(0, b->dimension - 1);  // generate random number between 0 and 9
    int dim2 = getRandNumber(0, b->dimension - 1);  // do it again
    int num = b->board[dim1][dim2];             // store number currently in random slot
    b->board[dim1][dim2] = 0;                   // remove number at randomly chosen slot by setting it to 0
    
    if (num && true) // solveBoard(b)                          // check if board created is unique
      numRemoved+=1;

    // reset the item changed to original and run thu loop again
    else {                                        
      b->board[dim1][dim2] = num;
    }

    // printf("Current state: board[%i][%i] = %i, numRemoved: [%i]\n", dim1, dim2, b->board[dim1][dim2], numRemoved);
  }

  return true;
}


/************ getRandomNumber ************/
/*
* Generates a random number within the range specified by the parameters and returns to caller
*
* Caller provides:
*   Two ints indicating the minimum and the maximum of the range of numbers in which
*   a random number should be generated
* We guarantee
*   A random int within the specified range is returnned
* Caller is responsible for:
*   Nothing
*/
static int getRandNumber(int min, int max) {
  int returnValue = rand() % (max - min + 1) + min;
  // printf("Rand generate [%i]\n", returnValue);
  return (returnValue);
}

/************ solveBoard ************/
/*
 * Solves a passed board and returns whether or not the solution is unique
 * 
 * Caller provides:
 *  A valid sudoku board
 * We guarantee:
 *  The passed board is solved with one solution or false is returned
 * Caller is responsible for:
 *  Nothing
 */


/**
 * Check how many zeros exist
 *  If none, return true
 * 
 * Loop through every number to insert
 *  Loop through every row
 *    Check 
 */

bool solveBoard(sudoku_t *b) {

    // count the number of cells to fill
    int cellsToFill = 0;

    // if there is a cell value with value 0, increment cellsToFill
    for (int i = 0; i < b->dimension; i++){
      for (int j = 0; j < b->dimension; j++){
        if (b->board[i][j] == 0){
          cellsToFill++;
        }
      }
    }

    // if the number if cellsToFill is 0; then the puzzle is solved, return true.
    if (cellsToFill == 0){
      return true;
    }

    int newValue;         // value to fill empty cells (should be between 1 and 9)
    int row = 0;            // keeps track of rows
    int col = 0;            // keeps track of columns

    // check for every possible new value entry
    for (newValue = 1; newValue <= b->dimension; newValue++){

      // iterate over the current row and check if the value already exists
      for (int i = 0; i < b->dimension; i++){
        if (b->board[row][i] == newValue){
            break;       // do nothing if the value already exists
        }
      }

      // iterate over the current column and check if the value already exists
      for (int j = 0; j < b->dimension; j++){
        if (b->board[j][col] == newValue){
          break;       // do nothing
        }
      }

      // iterate over the 3x3 squared region and check if the value already exists
      int squareRow = (row / 3) * 3;
      int squareCol = (col / 3) * 3;

      for (int i = squareRow; i < squareRow + 3; i++){
        for (int j = squareCol; j < squareCol + 3; j++){
          if (b->board[i][j] == newValue){
              continue;    // do nothing is the value already exists
          }
        }
      }

      // else fill the cell with the new value
      b->board[row][col] = newValue;
      
      // if the solving was done correctly, return true
      if (solveBoard(b)){
        return true;
      }

      // else, change that value to 0
      b->board[row][col] = 0;

    }

    return false;


  /*
  if (!b) return false;

  return false; */
}

bool testSolver(sudoku_t *b) {
  return testSolverHelper(b, 0);
}

bool testSolverHelper(sudoku_t *b, int pos) {

  // Check number of zeros
  int numZeros = 0;
  for (int i = 0; i < b->dimension; i++) {
    for (int j = 0; j < b->dimension; j++) {
      if (!b->board[i][j]) {
        numZeros++;
      }
    }
  }

  // Base case of recursion
  if (!numZeros) return true;

  // Find row and column from position
  int r = findArrayRow(b, pos);
  int c = findArrayCol(b, pos);

  // Step over filled numbers
  if (b->board[r][c]) return testSolverHelper(b, pos + 1);

  // Number to place
  int currentVal = 1;
  b->board[r][c] = currentVal;
  // printf("(%i, %i) -> [%i]\n", r, c, currentVal);

  while (isNumberPresent(b, r, c, currentVal) || !testSolverHelper(b, pos + 1)) {
    b->board[r][c] = ++currentVal;
    // printf("(%i, %i) -> [%i]\n", r, c, currentVal);

    if (currentVal > b->dimension) {
      b->board[r][c] = 0;
      return false;
    }
  }

  return true;
}

bool isNumberPresent(sudoku_t *b, int r, int c, int v) {
  if (checkRow(b, r, c, v)) { 
    // printf("Row invalid\n");
    return true;
  }

  if (checkCol(b, r, c, v)) {
    // printf("Col invalid\n");
    return true;
  }

  if (checkGrid(b, r, c, v)) {
    // printf("Grid invalid\n");
    return true;
  }

  return false;
}

bool checkRow(sudoku_t *b, int r, int c, int v) {
  for (int j = 0; j < b->dimension; j++) {
    // printf("ROW [%i] -> [%i] ?= [%i]\n", r, b->board[r][j], v);
    if (j == c) continue;
    if (b->board[r][j] == v) return true;
  }

  return false;
}

bool checkCol(sudoku_t *b, int r, int c, int v) {
  for (int i = 0; i < b->dimension; i++) {
    if (i == r) continue;
    if (b->board[i][c] == v) return true;
  }

  return false;
}

bool checkGrid(sudoku_t *b, int r, int c, int v) {
  int rStart = (int)(r / 3) * 3;
  int cStart = (int)(c / 3) * 3;

  for (int i = rStart; i < rStart + 3; i++) {
    for (int j = cStart; j < cStart + 3; j++) {
      if (i == r && j == c) continue;
      // printf("GRID (%i, %i) -> [%i] ?= [%i]\n", i, j, b->board[i][j], v);
      if (b->board[i][j] == v) return true;
    }
  }

  return false;
}

int findArrayRow(sudoku_t *b, int pos) {
  return (int)(pos / b->dimension);
}

int findArrayCol(sudoku_t *b, int pos) {
  return (int)(pos % b->dimension);
}
