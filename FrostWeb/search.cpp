/**
 * @file search.cpp
 * @brief Implementation of the search and evaluation functions for a chess engine.
 * 
 * This file contains the core algorithms that perform search operations in the chess game,
 * evaluating different positions, utilizing a transposition table to avoid redundant calculations,
 * and finding the best possible move based on the current game state. The search is based on
 * a recursive tree search mechanism, and optimizations like alpha-beta pruning are incorporated
 * to minimize search depth while ensuring optimal play.
 * 
 * The key functionalities include:
 * - Generating valid moves for a given position.
 * - Evaluating the board state.
 * - Searching through the game tree to return the best possible move.
 * - Using a transposition table to store previously computed board evaluations.
 * 
 * @date October 11, 2024
 * @author Anshuman Routray
 * 
 */

#include "search.hpp"
#include <iostream>

using Chessboard = std::vector<int>;

// Hash function for vector<int>
struct VectorHash {
    std::size_t operator()(const std::vector<int>& v) const {
        std::size_t hash = 0;
        for (int piece : v) {
            hash ^= std::hash<int>()(piece) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

unordered_map<Chessboard, double, VectorHash> savedPositions;

/**
 * The function `getValue` retrieves the value associated with a key in an unordered map, returning a
 * default value if the key is not found.
 * 
 * @param myMap An unordered map that stores values of type double associated with keys of type
 * Chessboard. The keys are hashed using a custom hash function VectorHash.
 * @param key The `key` parameter is of type `Chessboard`, which is used to look up a value in the
 * `myMap` unordered map.
 * 
 * @return If the key is found in the `myMap`, the associated value is being returned. Otherwise, the
 * default value of -3141.0 is being returned.
 */
double getValue(const std::unordered_map<Chessboard, double, VectorHash>& myMap, const Chessboard& key) {
    auto it = myMap.find(key);
    if (it != myMap.end()) {
        return it->second; // Return the associated value
    }
    return -3141.0; // Return default value if key not found
}


/**
 * The function `createChessBoard` converts a 2D vector representing a chessboard into a 1D vector.
 * 
 * @param board The `board` parameter is a 2D vector representing a chessboard with dimensions 8x8.
 * Each element in the vector represents a square on the chessboard and contains an integer value. The
 * function `createChessBoard` takes this 2D vector and converts it into a 1
 * 
 * @return The function `createChessBoard` returns a `Chessboard` object, which is created by iterating
 * over a 2D vector `board` and pushing its elements into the `newBoard` vector.
 */
Chessboard createChessBoard(vector<vector<int>> board){
    Chessboard newBoard;
    newBoard.reserve(64);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            newBoard.push_back(board[i][j]);
        }
    }
    return newBoard;
}

double stable_search(vector<vector<int>> board){
    int player = board[8][2];
    int pos1 = board[8][3];
    int pos2 = board[8][4];
    board[8][2] = -player;
    if(!isAttacked(board, {pos1, pos2})){
        return evaluate(board);      
    }
    board[8][2] = player; 
    vector<vector<vector<int>>> moveList = generateMoves(board);
    int minValue = 1000;
    vector<vector<int>> nextBoard; 
    for(vector<vector<int>> newBoard : moveList){
        if(newBoard[pos1][pos2] * player > 0 && pieceValues[abs(newBoard[pos1][pos2])] < minValue){
            minValue = pieceValues[abs(newBoard[pos1][pos2])];
            nextBoard = newBoard;
        }
    }
    if(nextBoard.empty()){
        cerr << "WARNING -- UNEXPECTED BEHAVIOR: NEXT BOARD IS EMPTY" << endl;
        cerr << moveList.size() << endl;
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                std::cout << board[i][j] << "  ";
            }
            std::cout << endl;
        }
        std::cout << board[8][0] << " " << board[8][1] << " " << board[8][2] << " " << board[8][3] 
            << " " << board[8][4] << " " << board[8][5];
        
        std::cout << endl << endl;
    }
    nextBoard[8][2] = -nextBoard[8][2];
    if(pieceValues[abs(board[pos1][pos2])] < minValue && isAttacked(nextBoard, {pos1, pos2})){
        return evaluate(board);
    }
    nextBoard[8][2] = -nextBoard[8][2];
    double evaluation = stable_search(nextBoard);
    return evaluation;
}

double search(vector<vector<int>> board, int depth, double bestOfWhite, double bestOfBlack){
    int player = board[8][2];
    vector<vector<vector<int>>> moveList = generateMoves(board);

    //If no legal moves are possible
    if(moveList.size() == 0){

        //Check if the king is under attack

        pair<int, int> kingPos = retrieveKingPosition(board, player);
        if(isAttacked(board, kingPos)){
            return -player * 100.0;
        }
        else {
            return 0.0;
        }
    }

    Chessboard hashedBoard = createChessBoard(board);

    if(getValue(savedPositions, hashedBoard) != -3141.0){
        return savedPositions[hashedBoard];
    }

    if(depth == 0){
        double evaluation = stable_search(board);
        return evaluation;
    }
    double evaluation = 0;
    if(player == whitePlayer){
        evaluation = -1000.0;
        int count = 0;
        for(vector<vector<int>> newBoard : moveList){
            double newEvaluation = search(newBoard, depth - 1, bestOfWhite, bestOfBlack);
            evaluation = max(evaluation, newEvaluation);
            bestOfWhite = max(bestOfWhite, newEvaluation);
            if(bestOfBlack <= bestOfWhite){
                break;
            }
        }
    }
    else {
        evaluation = 1000.0;
        for(vector<vector<int>> newBoard : moveList){   
            double newEvaluation = search(newBoard, depth - 1, bestOfWhite, bestOfBlack);
            evaluation = min(evaluation, newEvaluation);
            bestOfBlack = min(bestOfBlack, newEvaluation);
            if(bestOfBlack <= bestOfWhite){
                break;
            }
        }
    }

    savedPositions[hashedBoard] = evaluation;

    return evaluation;
}

vector<vector<int>> getBestMove(vector<vector<int>> board, int depth){
    vector<vector<vector<int>>> moveList = generateMoves(board);
    int player = board[8][2];
    if(moveList.size() == 0){
        
        //Check if the king is under attack

        pair<int, int> kingPos = retrieveKingPosition(board, player);
        if(isAttacked(board, kingPos)){
            return {{player}};
        }
        else {
            return {{0}};
        } 
    }
    double bestOfWhite = -1000.0, bestOfBlack = 1000.0;
    double bestEval = (player == whitePlayer) ? -1000.0 : 1000.0;
    vector<vector<int>> bestBoard;
    for(vector<vector<int>> newBoard : moveList){
        double evaluation = search(newBoard, depth - 1, bestOfWhite, bestOfBlack);
        if(player == whitePlayer){
            bestOfWhite = max(evaluation, bestOfWhite);
            if(evaluation > bestEval){
                bestEval = evaluation;
                bestBoard = newBoard;
            }
        }
        else {
            bestOfBlack = min(evaluation, bestOfBlack);
            if(evaluation < bestEval){
                bestEval = evaluation;
                bestBoard = newBoard;
            }
        }
    }
    return bestBoard;
}