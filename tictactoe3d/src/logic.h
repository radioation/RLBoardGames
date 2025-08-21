#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "genesis.h"

#define BOARD_SIZE 4


typedef enum {
    NO_PLAYER = 0,
    PLAYER_ONE = 1,
    PLAYER_TWO = 2,
} PLAYER;


bool check_win(s16 board[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE], u8 player);
bool is_a_draw(s16 board[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE]);

#endif // _LOGIC_H_
