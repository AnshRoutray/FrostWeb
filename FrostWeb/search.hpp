/**
 * @file search.hpp
 * @brief Declaration of search-related functions for the chess engine.
 * 
 * This header file contains the declarations for the search algorithms used to evaluate chess positions 
 * and determine optimal moves. It includes functions such as `search()`, `stable_search()`, `getBestMove()`, 
 * and `generateMoves()` that are essential for analyzing the game state and selecting moves.
 * 
 * The file also defines a custom hash function (`VectorHash`) for hashing the chessboard state and storing 
 * previously evaluated positions in an unordered map to speed up the search process.
 */

#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "board.hpp"
#include "evaluate.hpp"
#include <unordered_map>

/**
 * @brief The function `stable_search` recursively evaluates stable positions in a chess game to find the best
 * move.
 * 
 * @param board Recursively searching for a stable position in the game
 * state to evaluate the board.
 * 
 * @return  `double` value, which is the evaluation of the board
 * after performing a stable search.
 */
double stable_search(vector<vector<int>> board);

/**
 * @brief Searches through the game tree and returns the evaluation of a position
 *  
 * @param board: The current chessboard
 * 
 * @param depth: The amount of turns into the game the function will search through
 * 
 * @return The final evaluation of the position
 */
double search(vector<vector<int>> board, int depth, double bestOfWhite, double bestOfBlack);

/**
 * @brief Returns the best possible move possible out of all legal moves.
 * 
 * Utilizes the search function to determine the evaluation of each position
 * 
 * @param board: The chessboard represntation
 * 
 * @param depth: The depth to search at
 * 
 * @returns A chessboard with the best move played, if the game is already won or lost, then it returns 1, 0, or -1.
 * 
 */
vector<vector<int>> getBestMove(vector<vector<int>> board, int depth);

#endif