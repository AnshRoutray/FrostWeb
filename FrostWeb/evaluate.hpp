/**
 * @file evaluate.hpp
 * 
 * @brief This file contains the evaluation function for a chess engine.
 * 
 * Decleares the evaluation function
 * 
 * @author Anshuman Routray
 * @date October 11th, 2024
 */

#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include <vector>
#include <cmath>

using namespace std;

extern const vector<int> pieceValues;

/**
 * @brief An evaluation function for a chessboard
 * 
 * This function loops through the entire board and caluclates the final evaluation of the position.
 * Negative means black is winning, positive means white is winning.
 * 
 * @param board: This is the chessboard
 * 
 * @return A double value representing the evaluation of the board.
 */
double evaluate(vector<vector<int>> board);

#endif