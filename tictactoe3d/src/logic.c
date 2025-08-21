#include "logic.h"


// Function to check for a win condition
bool check_win(s16 board[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE], u8 player) {
    ////////////////////////////////////////////////////////////
    // same Z-layer checks
    // Check rows and cols in each layer 
    for (s16 z = 0; z < BOARD_SIZE; z++) {
        // Row check
        for (s16 y = 0; y < BOARD_SIZE; y++) {
            if (board[0][y][z] == player && board[1][y][z] == player &&
                board[2][y][z] == player && board[3][y][z] == player) {
                return true;
            }
        }
        // column check
        for (s16 x = 0; x < BOARD_SIZE; x++) {
            if (board[x][0][z] == player && board[x][1][z] == player &&
                board[x][2][z] == player && board[x][3][z] == player) {
                return true;
            }
        }
    }

    // Check diagonals in each layer
    for (s16 z = 0; z < BOARD_SIZE; z++) {
        if (board[0][0][z] == player && board[1][1][z] == player &&
            board[2][2][z] == player && board[3][3][z] == player) {
            return true;
        }
        if (board[3][0][z] == player && board[2][1][z] == player &&
            board[1][2][z] == player && board[0][3][z] == player) {
            return true;
        }
    }

    ////////////////////////////////////////////////////////////
    // Multi layer checks
    // Check straight down.
    for (s16 y = 0; y < BOARD_SIZE; y++) {
        for (s16 x = 0; x < BOARD_SIZE; x++) {
            if (board[x][y][0] == player && board[x][y][1] == player &&
                board[x][y][2] == player && board[x][y][3] == player) {
                return true;
            }
        }
    }

    // Check corner-to-corner 3D diagonals through the layers
    if (board[0][0][0] == player && board[1][1][1] == player &&  
        board[2][2][2] == player && board[3][3][3] == player) {
        return true;
    }
    if (board[3][0][0] == player && board[2][1][1] == player &&
        board[1][2][2] == player && board[0][3][3] == player) {
        return true;
    }
    if (board[0][3][0] == player && board[1][2][1] == player &&
        board[2][1][2] == player && board[3][0][3] == player) {
        return true;
    }
    if (board[3][3][0] == player && board[2][2][1] == player &&
        board[1][1][2] == player && board[0][0][3] == player) {
        return true;
    }

    // check top-rear to bottom-front
    for (s16 x = 0; x < BOARD_SIZE; x++) {
        if (board[x][0][0] == player && board[x][1][1] &&
            board[x][2][2] == player && board[x][3][3] ) {
            return true;
        }
    } 

    // check top-front to bottom-rear
    for (s16 x = 0; x < BOARD_SIZE; x++) {
        if (board[x][3][0] == player && board[x][2][1] &&
            board[x][1][2] == player && board[x][0][3] ) {
            return true;
        }
    } 

    // check top-left to bottom-right
    for (s16 y = 0; y < BOARD_SIZE; y++) {
        if (board[0][y][0] == player && board[1][y][1] &&
            board[2][y][2] == player && board[3][y][3] ) {
            return true;
        }
    } 
    

    // check top-right to bottom-left
    for (s16 y = 0; y < BOARD_SIZE; y++) {
        if (board[3][y][0] == player && board[2][y][1] &&
            board[1][y][2] == player && board[0][y][3] ) {
            return true;
        }
    } 
    
   
    return false;
}

// Function to check if the board is full (draw)
bool is_a_draw(s16 board[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE]) {
    for (s16 z = 0; z < BOARD_SIZE; z++) {
        for (s16 y = 0; y < BOARD_SIZE; y++) {
            for (s16 x = 0; x < BOARD_SIZE; x++) {
                if (board[x][y][z] == 0) {
                    // empty spot, not a draw. 
                    return false; 
                }
            }
        }
    }
    return true;
}

