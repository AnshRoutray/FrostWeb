/**
 * @file board.hpp
 * @brief Header file for chess engine functions related to the chessboard.
 * 
 * This header file declares various functions for managing a chess board,
 * including move generation, move validation, attack detection, and board evaluation. 
 * The chessboard is represented as a 2D vector, where each piece is denoted 
 * by a specific integer value. The file also includes constants to represent 
 * player colors, facilitating easy reference throughout the chess engine.
 * 
 * @author Anshuman Routray
 * @date August 24, 2024
 */


#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

// Constants for players and pieces

extern const int blackPlayer;
extern const int whitePlayer;

extern const int space;
extern const int pawn; 
extern const int knight;
extern const int bishop;
extern const int rook;
extern const int queen;
extern const int king;

extern const int bothCastlingEnabled;
extern const int bothCastlingDisabled;
extern const int shortCastlingDisabled;
extern const int longCastlingDisabled;

extern const vector<vector<int>> startingBoard;

/**
 * @brief This function returns the position of the king in the game
 * 
 * @param board: This is the chessboard
 * 
 * @param player: This holds which player's king you want to 
 * find the position of (1 for white, -1 for black)
 * 
 * @returns A pair representing the corrdinates of the position of the king.
 */

pair<int, int> retrieveKingPosition(vector<vector<int>> board, int player);

/**
 * @brief Executes a move on the board.
 * 
 * @param board The current board state.
 * 
 * @param initialPosition The starting position of the piece.
 * 
 * @param finalPosition The ending position of the piece.
 * 
 * @param promotionPiece The piece to which a pawn is promoted (if applicable).
 * 
 * @param enPassant Whether the move is an enPassant move.
 * 
 * @return The new board state after the move.
 */
vector<vector<int>> playMove(const vector<vector<int>> board, 
    pair<int, int> initialPosition, pair<int, int> finalPosition, int promotionPiece, bool enPassant);

/**
 * @brief Checks if a square is under attack.
 * 
 * @param board The chess board.
 * 
 * @param position The position to check.
 * 
 * @return True if the square is attacked, otherwise false.
 */
bool isAttacked(const vector<vector<int>> board, pair<int, int> position);

/**
 * @brief Generates all possible moves for the current player.
 * 
 * @param board The current board state.
 * 
 * @return A vector of boards representing possible moves.
 */
vector<vector<vector<int>>> generateMoves(const vector<vector<int>> board);

#endif