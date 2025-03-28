/**
 * @file evaluate.cpp
 * @brief This file contains the evaluation function for a chess engine.
 * 
 * The evaluation function calculates the score of a given chessboard state based on
 * material and positional values of the pieces. Positive values indicate an advantage
 * for White, while negative values favor Black.
 * 
 * @author Anshuman Routray
 * @date October 11th, 2024
 */

#include "evaluate.hpp"

using namespace std;

//defining the piece values

const vector<int> pieceValues = {0, 1, 3, 3, 5, 9, 100};

double piecePos[7][8][8] = {
    // Pawn positions
    {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
        },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1},
        {0.05, 0.05, 0.1, 0.2, 0.2, 0.1, 0.05, 0.05},
        {0.05, 0.05, 0.1, 0.3, 0.3, 0.1, 0.05, 0.05},
        {0.0, 0.0, 0.0, 0.3, 0.3, 0.0, 0.0, 0.0},
        {0.05, -0.05, -0.1, 0.0, 0.0, -0.1, -0.05, 0.05},
        {0.05, 0.1, 0.1, -0.2, -0.2, 0.1, 0.1, 0.05},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    },
    // Knight positions
    {
        {-0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5},
        {-0.4, -0.2, 0.0, 0.0, 0.0, 0.0, -0.2, -0.4},
        {-0.3, 0.0, 0.1, 0.15, 0.15, 0.1, 0.0, -0.3},
        {-0.3, 0.05, 0.15, 0.2, 0.2, 0.15, 0.05, -0.3},
        {-0.3, 0.0, 0.15, 0.2, 0.2, 0.15, 0.0, -0.3},
        {-0.3, 0.05, 0.1, 0.15, 0.15, 0.1, 0.05, -0.3},
        {-0.4, -0.2, 0.0, 0.05, 0.05, 0.0, -0.2, -0.4},
        {-0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5}
    },
    // Bishop positions
    {
        {-0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2},
        {-0.1, 0.05, 0.0, 0.0, 0.0, 0.0, 0.05, -0.1},
        {-0.1, 0.1, 0.05, 0.05, 0.05, 0.05, 0.1, -0.1},
        {-0.1, 0.0, 0.05, 0.1, 0.1, 0.05, 0.0, -0.1},
        {-0.1, 0.05, 0.05, 0.1, 0.1, 0.05, 0.05, -0.1},
        {-0.1, 0.0, 0.05, 0.05, 0.05, 0.05, 0.0, -0.1},
        {-0.1, 0.05, 0.0, 0.0, 0.0, 0.0, 0.05, -0.1},
        {-0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2}
    },
    // Rook positions
    {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.05, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.05},
        {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
        {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
        {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
        {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
        {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
        {0.0, 0.0, 0.0, 0.05, 0.05, 0.0, 0.0, 0.0}
    },
    // Queen positions (all zero values) 
    {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    },
    // General King positions
    {
        {-0.3, -0.4, -0.2, -0.1, -0.1, -0.2, -0.4, -0.3},
        {-0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2, -0.3},
        {-0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.3},
        {-0.4, -0.4, -0.5, -0.5, -0.5, -0.5, -0.4, -0.3},
        {-0.2, -0.3, -0.3, -0.4, -0.4, -0.3, -0.3, -0.2},
        {-0.1, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.1},
        {0.3, 0.4, -0.2, -0.1, -0.1, -0.2, 0.4, 0.3},
        {0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.3, 0.2}
    }
};
 
double evaluate(vector<vector<int>> board){
    double evaluation = 0;
    for(int row = 0; row < 8; row++){
        for(int column = 0; column < 8; column++){
            int piece = board[row][column];
            evaluation += ((piece > 0) ? (pieceValues[abs(piece)]) : (-pieceValues[abs(piece)]));
            evaluation += (piece > 0) ? (piecePos[abs(piece)][row][column]) : (-piecePos[abs(piece)][row][column]);
        } 
    }
    return evaluation;
}