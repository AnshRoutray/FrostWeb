/**
 * @file board.cpp
 * @brief Implementation of the chess engine functions.
 * 
 * This file contains the implementation of various functions for a chess board,
 * including move generation, move validation, and attack detection. The functions
 * handle different aspects of chess gameplay such as piece movement, castling,
 * and detecting whether a square is attacked.
 * 
 * The chess board is represented using a 2D vector where each piece is denoted
 * by a specific integer value. The 9th row is the metadata regarding the state of the board.
 * The board supports castling, en passant, pawn 
 * promotion, and other standard chess rules.
 * 
 * The implementation focuses on maintaining the board state and generating all 
 * possible legal moves.
 * 
 * @author Anshuman Routray
 * @date August 24, 2024
 */

#include "board.hpp"

using namespace std;

//Defining player signature

const int blackPlayer = -1;
const int whitePlayer = 1; 

//Defining piece signatures

const int space = 0;
const int pawn = 1;
const int knight = 2; 
const int bishop = 3;
const int rook = 4;
const int queen = 5;
const int king = 6;

//Castling States

const int bothCastlingEnabled = 0;
const int bothCastlingDisabled = 1;
const int shortCastlingDisabled = 2;
const int longCastlingDisabled = 3;

/*Defining starting board, last subArray denotes castling ability for black and white, player's turn, final
position of last played move, and whether the last move was a double space pawn move(1 for yes, 0 for no) for En Passant
respectively.*/

const vector<vector<int>> startingBoard = 
    {{-rook, -knight, -bishop, -queen, -king, -bishop, -knight, -rook}, 
     {-pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn},
     {space, space, space, space, space, space, space, space},
     {space, space, space, space, space, space, space, space},
     {space, space, space, space, space, space, space, space},
     {space, space, space, space, space, space, space, space},
     {pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn},
     {rook, knight, bishop, queen, king, bishop, knight, rook},
     {bothCastlingEnabled, bothCastlingEnabled, whitePlayer, -1, -1, 0}};

pair<int, int> retrieveKingPosition(vector<vector<int>> board, int player){
    pair<int, int> kingPos;
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            if(board[r][c] == king * player){
                kingPos = {r, c};
            }
        }
    }
    return kingPos;
}

vector<vector<int>> playMove(vector<vector<int>> board,
    pair<int, int> initialPosition, pair<int, int> finalPosition, int promotionPiece, bool enPassant){
        int player = board[8][2];
        int castlingState = (player == blackPlayer) ? (0) : (1);
        int piece = abs(board[initialPosition.first][initialPosition.second]);
        board[initialPosition.first][initialPosition.second] = space;
        board[finalPosition.first][finalPosition.second] = piece * player;
        board[8][3] = finalPosition.first;
        board[8][4] = finalPosition.second;
        board[8][5] = 0;
        if(enPassant){
            board[initialPosition.first][finalPosition.second] = space;
        }
        else if(promotionPiece != 0){
            board[finalPosition.first][finalPosition.second] = promotionPiece * player;
        }

        if(piece == pawn && abs(finalPosition.first - initialPosition.first) == 2){
            board[8][5] = 1;
        }
        else if(piece == king){
            board[8][castlingState] = bothCastlingDisabled;
            if(finalPosition.second - initialPosition.second == 2){
                board[initialPosition.first][5] = rook;
                board[initialPosition.first][7] = space;
            }
            else if(finalPosition.second - initialPosition.second == -2){
                board[initialPosition.first][3] = rook;
                board[initialPosition.first][0] = space;
            }
        }
        else if(piece == rook && board[8][castlingState] != bothCastlingDisabled){

            // floor(3.5 + 3.5 * player) returns the row of the King depending on Black or White

            if(initialPosition.first == floor(3.5 + 3.5 * player) && initialPosition.second == 0){
                board[8][castlingState] = 
                    (board[8][castlingState] == shortCastlingDisabled) ? bothCastlingDisabled : longCastlingDisabled;
            }
            else if(initialPosition.first == floor(3.5 + 3.5 * player) && initialPosition.second == 7){
                board[8][castlingState] = 
                    (board[8][castlingState] == longCastlingDisabled) ? bothCastlingDisabled : shortCastlingDisabled;
            }
        }

    //Checking if move is an illegal move, if true then returning an empty board

    //Finding Position of Kings

    pair<int, int> whiteKingPos = {0, 0};
    pair<int, int> blackKingPos = {0, 0};

    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            if(board[r][c] == king){
                whiteKingPos = {r, c};
            }
            else if(board[r][c] == -king){
                blackKingPos = {r, c};
            }
        }
    } 

    if(player == whitePlayer){

        //If white can attack black's king, then the previous move was illegal

        if(isAttacked(board, whiteKingPos)){
            return {};
        }
    }
    else {

        //If black can attack white's king, then the previous move was illegal

        if(isAttacked(board, blackKingPos)){
            return {};
        }
    }

    board[8][2] = -player;

    return board;
}

bool isAttacked(vector<vector<int>> board, pair<int, int> position){
    int row = position.first;
    int column = position.second;
    int player = board[8][2];
    int piece = abs(board[row][column]);
    board[8][2] = -player;
    //upper right diagonal

    int rowChange = row - 1, columnChange = column + 1;
    while(rowChange >= 0 && columnChange < 8 && 
        board[rowChange][columnChange] * player <= 0){
        if(board[rowChange][columnChange] == -(bishop * player)
            || board[rowChange][columnChange] == -(queen * player)){
            if(piece != king && playMove(board, {rowChange, columnChange}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
        else if(board[rowChange][columnChange] != space){
            break;
        }
        rowChange--, columnChange++;
    }

    //upper left diagonal

    rowChange = row - 1, columnChange = column - 1;
    while(rowChange >= 0 && columnChange >= 0 && 
        board[rowChange][columnChange] * player <= 0){
        if(board[rowChange][columnChange] == -(bishop * player)
            || board[rowChange][columnChange] == -(queen * player)){
            if(piece != king && playMove(board, {rowChange, columnChange}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
        else if(board[rowChange][columnChange] != space){
            break;
        }
        rowChange--, columnChange--;
    }

    //bottom right diagonal

    rowChange = row + 1, columnChange = column + 1;
    while(rowChange < 8 && columnChange < 8 && 
        board[rowChange][columnChange] * player <= 0){
        if(board[rowChange][columnChange] == -(bishop * player)
            || board[rowChange][columnChange] == -(queen * player)){
            if(piece != king && playMove(board, {rowChange, columnChange}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
        else if(board[rowChange][columnChange] != space){
            break;
        }
        rowChange++, columnChange++;
    }

    //bottom left diagonal

    rowChange = row + 1, columnChange = column - 1;
    while(rowChange < 8 && columnChange >= 0 && 
        board[rowChange][columnChange] * player <= 0){
        if(board[rowChange][columnChange] == -(bishop * player)
            || board[rowChange][columnChange] == -(queen * player)){
            if(piece != king && playMove(board, {rowChange, columnChange}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
        else if(board[rowChange][columnChange] != space){
            break;
        }
        rowChange++, columnChange--;
    }

    //upper line

    rowChange = row - 1, columnChange = column;
    while(rowChange >= 0 && board[rowChange][columnChange] * player <= 0){
        if(board[rowChange][columnChange] == -(rook * player) || 
        board[rowChange][columnChange] == -(queen * player)){
            if(piece != king && playMove(board, {rowChange, columnChange}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
        else if(board[rowChange][columnChange] != space){
            break;
        }
        rowChange--;
    }

    //bottom line

    rowChange = row + 1, columnChange = column;
    while(rowChange < 8 && board[rowChange][columnChange] * player <= 0){
        if(board[rowChange][columnChange] == -(rook * player) || 
        board[rowChange][columnChange] == -(queen * player)){
            if(piece != king && playMove(board, {rowChange, columnChange}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
        else if(board[rowChange][columnChange] != space){
            break;
        }
        rowChange++;
    }

    //right line

    rowChange = row, columnChange = column + 1;
    while(columnChange < 8 && board[rowChange][columnChange] * player <= 0){
        if(board[rowChange][columnChange] == -(rook * player) || 
        board[rowChange][columnChange] == -(queen * player)){
            if(piece != king && playMove(board, {rowChange, columnChange}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
        else if(board[rowChange][columnChange] != space){
            break;
        }
        columnChange++;
    }

    //left line

    rowChange = row, columnChange = column - 1;
    while(columnChange >= 0 && board[rowChange][columnChange] * player <= 0){
        if(board[rowChange][columnChange] == -(rook * player) || 
        board[rowChange][columnChange] == -(queen * player)){
            if(piece != king && playMove(board, {rowChange, columnChange}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
        else if(board[rowChange][columnChange] != space){
            break;
        }
        columnChange--;
    }

    //All possible squares a knight can attack from

    vector<pair<int, int>> possibleKnightMoves = {{row - 2, column + 1}, {row - 2, column - 1}, 
        {row + 2, column + 1}, {row + 2, column - 1}, {row + 1, column + 2}, {row + 1, column - 2},
        {row - 1, column + 2}, {row - 1, column - 2}};
    for(pair<int, int> move : possibleKnightMoves){
        if(move.first < 8 && move.first >= 0 && move.second < 8 && move.second >= 0 && 
            board[move.first][move.second] == -(knight * player)){
            if(piece != king && playMove(board, {move.first, move.second}, position, 0, false).empty()){
                break;
            }
            else {
                return true;
            }
        }
    }

    //Pawn Attacks

    if(player == blackPlayer){
        if(row < 7 && column > 0 && board[row + 1][column - 1] == pawn && 
           piece != king && !playMove(board, {row + 1, column - 1}, position, 0, false).empty()){
            return true;
        }
        else if(row < 7 && column < 7 && board[row + 1][column + 1] == pawn && 
            piece != king && !playMove(board, {row + 1, column + 1}, position, 0, false).empty()){
            return true;
        }
    }
    else {
        if(row > 0 && column < 7 && board[row - 1][column + 1] == -pawn && 
            piece != king && !playMove(board, {row - 1, column + 1}, position, 0, false).empty()){
            return true;
        }
        else if(row > 0 && column > 0 && board[row - 1][column - 1] == -pawn && 
            piece != king && !playMove(board, {row - 1, column - 1}, position, 0, false).empty()) {
            return true;
        }
    }

    //King Attacks

    for(int i = row - 1; i <= row + 1; i++){
        for(int j = column - 1; j <= column + 1; j++){
            if(i >= 0 && i < 8 && j >= 0 && j < 8 && board[i][j] == (king * player)){
                return false;
            }
            if(i >= 0 && i < 8 && j >= 0 && j < 8 && board[i][j] == -(king * player)){
                if(!playMove(board, {i, j}, position, 0, false).empty()){
                    return true;
                }
            }
        }
    }
    return false;
}

vector<vector<vector<int>>> generateMoves(vector<vector<int>> board){
    vector<vector<vector<int>>> moveList;
    int player = board[8][2];
    for(int row = 0; row < 8; row++){
        for(int column = 0; column < 8; column++){
            int piece = board[row][column];
            pair<int, int> previousMove = {board[8][3], board[8][4]};
            if(player * piece > 0){              
                if(abs(piece) == pawn){
                    //Checking for promotion ability of pawn, else checks through normal pawn movements

                    if((player == whitePlayer && row == 1) || (player == blackPlayer && row == 6)){
                        //Going through all possible promotion options, and all possible movement of pawn

                        for(int promoteTo = queen; promoteTo >= knight; promoteTo--){
                            if(board[row - player][column] == space){
                            moveList.push_back(playMove(board, {row, column}, 
                                {row - player, column}, promoteTo, false));
                            }
                            if(column < 7 && board[row - player][column + 1] * player < 0){
                            moveList.push_back(playMove(board, {row, column}, 
                                {row - player, column + 1}, promoteTo, false));
                            }
                            if(column > 0 && board[row - player][column - 1] * player < 0){
                            moveList.push_back(playMove(board, {row, column}, 
                                {row - player, column - 1}, promoteTo, false));
                            }
                        }
                    }
                    else{
                        //Normal Movement of Pawn
                        if(board[row - player][column] == space){
                            moveList.push_back(playMove(board, {row, column}, 
                                {row - player, column}, 0, false)); //PROBLEMATIC!! WHY???
                        }
                        if(row - player < 7 && row - player > 0 && column > 0 && board[row - player][column - 1] * player < 0){
                            moveList.push_back(playMove(board, {row, column}, 
                                {row - player, column - 1}, 0, false));
                        } 
                        if(row - player < 7 && row - player > 0 && column < 7 && board[row - player][column + 1] * player < 0){
                            moveList.push_back(playMove(board, {row, column}, 
                                {row - player, column + 1}, 0, false));
                        }   
                    }
                    //En Passant and Double Space Pawn Move Respectively

                    if(board[8][5] == 1 && previousMove.first == row && abs(previousMove.second - column) == 1){
                        moveList.push_back(playMove(board, {row, column}, 
                            {previousMove.first - player, previousMove.second}, 0, true));
                    }
                    if(((player == whitePlayer && row == 6) || (player == blackPlayer && row == 1)) && 
                        board[row - player * 2][column] == space && board[row - player][column] == space){
                        moveList.push_back(playMove(board, {row, column}, 
                            {row - player * 2, column}, 0, false));
                    }
                }         
                else if(abs(piece) == knight){

                    //All possible squares a knight can move to

                    vector<pair<int, int>> possibleKnightMoves = {{row - 2, column + 1}, {row - 2, column - 1}, 
                        {row + 2, column + 1}, {row + 2, column - 1}, {row + 1, column + 2}, {row + 1, column - 2},
                        {row - 1, column + 2}, {row - 1, column - 2}};
                    for(pair<int, int> move : possibleKnightMoves){
                        if(move.first < 8 && move.first >= 0 && move.second < 8 && move.second >= 0 && 
                            board[move.first][move.second] * player <= 0){
                            moveList.push_back(playMove(board, {row, column}, 
                                {move.first, move.second}, 0, false));
                        }
                    }
                }
                if(abs(piece) == bishop || abs(piece) == queen){
                    
                    //upper right diagonal for bishop or queen

                    int rowChange = row - 1, columnChange = column + 1;
                    while(rowChange >= 0 && columnChange < 8 && 
                        board[rowChange][columnChange] * player <= 0){
                        moveList.push_back(playMove(board, {row, column}, 
                            {rowChange, columnChange}, 0, false));
                        if(board[rowChange][columnChange] * player < 0){
                            break;
                        }
                        rowChange--, columnChange++;
                    }

                    //upper left diagonal for bishop or queen

                    rowChange = row - 1, columnChange = column - 1;
                    while(rowChange >= 0 && columnChange >= 0 && 
                        board[rowChange][columnChange] * player <= 0){
                        moveList.push_back(playMove(board, {row, column}, 
                            {rowChange, columnChange}, 0, false));
                        if(board[rowChange][columnChange] * player < 0){
                            break;
                        }
                        rowChange--, columnChange--;
                    }

                    //bottom right diagonal for bishop or queen

                    rowChange = row + 1, columnChange = column + 1;
                    while(rowChange < 8 && columnChange < 8 && 
                        board[rowChange][columnChange] * player <= 0){
                        moveList.push_back(playMove(board, {row, column}, 
                            {rowChange, columnChange}, 0, false));
                        if(board[rowChange][columnChange] * player < 0){
                            break;
                        }
                        rowChange++, columnChange++;
                    }

                    //bottom left diagonal for bishop or queen

                    rowChange = row + 1, columnChange = column - 1;
                    while(rowChange < 8 && columnChange >= 0 && 
                        board[rowChange][columnChange] * player <= 0){
                        moveList.push_back(playMove(board, {row, column}, 
                            {rowChange, columnChange}, 0, false));
                        if(board[rowChange][columnChange] * player < 0){
                            break;
                        }
                        rowChange++, columnChange--;
                    }
                }
                if(abs(piece) == rook || abs(piece) == queen){

                    //upper line for rook or queen

                    int rowChange = row - 1, columnChange = column;
                    while(rowChange >= 0 && board[rowChange][columnChange] * player <= 0){
                        moveList.push_back(playMove(board, {row, column}, 
                            {rowChange, columnChange}, 0, false));
                        if(board[rowChange][columnChange] * player < 0){
                            break;
                        }
                        rowChange--;
                    }

                    //bottom line for rook or queen

                    rowChange = row + 1, columnChange = column;
                    while(rowChange < 8 && board[rowChange][columnChange] * player <= 0){
                        moveList.push_back(playMove(board, {row, column}, 
                            {rowChange, columnChange}, 0, false));
                        if(board[rowChange][columnChange] * player < 0){
                            break;
                        }
                        rowChange++;
                    }

                    //right line for rook or queen

                    rowChange = row, columnChange = column + 1;
                    while(columnChange < 8 && board[rowChange][columnChange] * player <= 0){
                        moveList.push_back(playMove(board, {row, column}, 
                            {rowChange, columnChange}, 0, false));
                        if(board[rowChange][columnChange] * player < 0){
                            break;
                        }
                        columnChange++;
                    }

                    //left line for rook or queen

                    rowChange = row, columnChange = column - 1;
                    while(columnChange >= 0 && board[rowChange][columnChange] * player <= 0){
                        moveList.push_back(playMove(board, {row, column}, 
                            {rowChange, columnChange}, 0, false));
                        if(board[rowChange][columnChange] * player < 0){
                            break;
                        }
                        columnChange--;
                    }
                }
                else if(abs(piece) == king){

            
                    //looping through all possible King Moves

                    for(int i = row - 1; i <= row + 1; i++){
                        for(int j = column - 1; j <= column + 1; j++){
                            if(i >= 0 && i < 8 && j >= 0 && j < 8 && board[i][j] * player <= 0){
                                moveList.push_back(playMove(board, {row, column}, {i, j}, 0, false));
                            }
                        }
                    }

                    //Short Castling
                    
                    int castlingState = board[8][floor(0.5 + player)];

                    if(!(castlingState == shortCastlingDisabled || castlingState == bothCastlingDisabled) &&
                        board[row][column + 1] == space && board[row][column + 2] == space && 
                        !isAttacked(board, {row, column}) && !isAttacked(board, {row, column + 1}) && 
                        !isAttacked(board, {row, column + 2}) && board[row][7] == 4){
                        moveList.push_back(playMove(board, {row, column}, {row, column + 2}, 0, false));
                    }

                    //Long Castling

                    if(!(castlingState == longCastlingDisabled || castlingState == bothCastlingDisabled) &&
                        board[row][column - 1] == space && board[row][column - 2] == space && 
                        board[row][column - 3] == space && !isAttacked(board, {row, column}) &&
                        !isAttacked(board, {row, column - 1}) && !isAttacked(board, {row, column - 2}) &&
                        !isAttacked(board, {row, column - 3}) && board[row][0] == 4){
                        moveList.push_back(playMove(board, {row, column}, {row, column - 2}, 0, false));
                    }                   
                }   
            }
        }
    }

    //Removing all illegal moves from list (empty boards)
    vector<vector<vector<int>>> legalMoveList;
    legalMoveList.reserve(50);
    for(vector<vector<int>> legalBoard : moveList){
        if(!legalBoard.empty()){ // If legal move
            legalMoveList.push_back(legalBoard);
        }
    }

    return legalMoveList;
}