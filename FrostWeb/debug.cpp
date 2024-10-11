/**
 * @file debug.cpp
 * 
 * @brief Provides a way to debug the FrostWeb Chess Engine
 * 
 * @author Anshuman Routray
 * 
 * @date October 11th, 2024
 */

#include <iostream>
#include "search.hpp"

using namespace std;

void printBoard(vector<vector<int>> board){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            cout << abs(board[i][j]) << " ";
        }
        cout << endl;
    }
    cout << board[8][0] << " " << board[8][1] << " " << board[8][2] << " " << board[8][3] 
        << " " << board[8][4] << " " << board[8][5];
    cout << endl << endl;
}

int main(){
    const vector<vector<int>> debugboard = 
    {{-rook, space, -bishop, -queen, -king, -bishop, -knight, -rook}, 
     {-pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn},
     {space, space, -knight, space, space, space, space, space},
     {space, space, space, space, space, space, space, space},
     {space, space, bishop, space, pawn, space, space, space},
     {space, space, space, space, space, queen, space, space},
     {pawn, pawn, pawn, pawn, space, pawn, pawn, pawn},
     {rook, knight, bishop, space, rook, space, space, rook},
     {longCastlingDisabled, bothCastlingEnabled, blackPlayer, 5, 5, 0}};

    return 0;
}