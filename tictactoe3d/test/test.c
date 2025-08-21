#include <assert.h>
#include "../src/logic.h"

short board[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE] = {0};

void clear_board() {
    memset( board, 0, sizeof(board));
}

void test_all_win_conditions(short player) {
    printf("Testing all win conditions for player %d...\n", player);
    int x, y, z;

    // Test rows (within layers)
    for (int z = 0; z < BOARD_SIZE; z++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            clear_board();
            for (int x = 0; x < BOARD_SIZE; x++)
                board[z][y][x] = player;
            assert(check_win(board, player));
        }
    }

    // Test columns (within layers)
    for (int z = 0; z < BOARD_SIZE; z++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            clear_board();
            for (int y = 0; y < BOARD_SIZE; y++)
                board[z][y][x] = player;
            assert(check_win(board, player));
        }
    }

    // Test verticals (same x, y across layers)
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            clear_board();
            for (int z = 0; z < BOARD_SIZE; z++)
                board[z][y][x] = player;
            assert(check_win(board, player));
        }
    }

    // Diagonals within each layer
    for (int z = 0; z < BOARD_SIZE; z++) {
        clear_board();
        for (int x = 0; x < BOARD_SIZE; x++)
            board[z][x][x] = player;
        assert(check_win(board, player));

        clear_board();
        for (int x = 0; x < BOARD_SIZE; x++)
            board[z][x][BOARD_SIZE - 1 - x] = player;
        assert(check_win(board, player));
    }

    // TODO: 3D diagonals

    // Negative test: make sure empty board returns false
    clear_board();
    assert(!check_win(board, player));

    printf("All win condition tests passed for player %d.\n", player);
}

int main( int argc, char* argv[] ) {
   test_all_win_conditions(1);
   test_all_win_conditions(2);
}
