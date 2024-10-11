/**
 * @brief Provides interface for using the FrostWeb Chess Engine
 * 
 * This file provides a way for other programs to use the FrostWeb Interface
 * It is meant to be compiled with the other programs and made to an executable
 * 
 * @author Anshuman Routray
 * @date October 11th 2024
 */

#include <iostream>
#include "search.hpp"

using namespace std;

const int DEPTH = 4; //Depth to search at

void printBoard(vector<vector<int>> board){
    if(board.size() == 1){
        cout << board[0][0];
        return;
    }
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << board[8][0] << " " << board[8][1] << " " << board[8][2] << " " << board[8][3] 
        << " " << board[8][4] << " " << board[8][5];
}

int main(){
 
    vector<vector<int>> inputBoard;
    int mode;

    cin >> mode;

    for(int i = 0; i < 8; i++){
        vector<int> row;
        for(int j = 0; j < 8; j++){
            int x;
            cin >> x;
            row.push_back(x);
        }
        inputBoard.push_back(row);
    }

    vector<int> row2;

    for(int i = 0; i < 6; i++){
        int x;
        cin >> x;
        row2.push_back(x);
    }

    inputBoard.push_back(row2);

    if(mode == 1){
        printBoard(getBestMove(inputBoard, DEPTH));
    }
    else if (mode == 2){
        int moves = generateMoves(inputBoard).size();
        if(moves == 0){
            pair<int, int> kingPos = retrieveKingPosition(inputBoard, inputBoard[8][2]);
            if(isAttacked(inputBoard, kingPos)){
                cout << -1;
            }
            else {
                cout << 0;
            }
        }
        else {
            cout << 100;
        }
    }
    return 0; 
} 