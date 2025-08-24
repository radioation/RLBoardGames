#include <assert.h>
#include "../src/logic.h"



#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define u8 unsigned char
#define u16 unsigned short
#define s16 short

u16 g_pass = 0;
u16 g_fail = 0;
#define EXPECT(expr) do{ if (expr){ g_pass++; printf("  PASS at %s:%d: %s\n", __FILE__,__LINE__, #expr); } else { g_fail++; \
    printf("  FAIL at %s:%d: %s\n", __FILE__, __LINE__, #expr); } }while(0)

extern CHESS_PIECE board[BOARD_SIZE][BOARD_SIZE];

void print_board() {
    printf("\n--------\n");
    printf("  0 1 2 3 4 5 6 7\n");
    for (int y=0;y<8;y++){
        printf("%d ", y );
        for (int x=0;x<8;x++){
            const CHESS_PIECE *p = &board[x][y];
            char c='.';
            switch(p->type){
                case KING: 
                    c='K';
                    break; 
                case QUEEN:
                    c='Q';
                    break;
                case ROOK:
                    c='R'; 
                    break;
                case BISHOP:
                    c='B'; 
                    break;
                case KNIGHT:
                    c='N'; 
                    break;
                case PAWN:
                    c='P'; 
                    break;
                default: 
                    c='.'; 
                    break;
            }
            if (p->player==PLAYER_TWO) c = (char)(c + 32); 

            printf("%c ", c);
        }
        printf("\n");
    }
}


void test_pawns() {
    printf("test_pawns ----------------------------------------------\n");
    clear_board();

    printf("test_pawns : setup board\n " );
    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(4,0,KING,PLAYER_TWO);

    // White pawn at (4,6) 
    set_piece(4,6,PAWN,PLAYER_ONE);
    print_board();

    // single space 
    printf("test_pawns : move white single - " );
    EXPECT(is_valid_move(4,6,4,5) == true);
    printf("test_pawns : move white double - " );
    EXPECT(is_valid_move(4,6,4,4) == true);

    // block forward
    printf("test_pawns : block white with white - " );
    set_piece(4,5,PAWN,PLAYER_ONE); // own piece blocks
    print_board();
    EXPECT(is_valid_move(4,6,4,5) == false); //single
    EXPECT(is_valid_move(4,6,4,4) == false); //double

    printf("test_pawns : block white with black - " );
    set_piece(4,5,PAWN,PLAYER_TWO); 
    print_board();
    EXPECT(is_valid_move(4,6,4,5) == false);// single
    EXPECT(is_valid_move(4,6,4,4) == false); //double

    // clear block, add enemy at diagonal for capture
    set_piece(4,5,EMPTY,NO_PLAYER);
    set_piece(3,5,PAWN,PLAYER_TWO);
    set_piece(5,5,PAWN,PLAYER_TWO);
    print_board();
    printf("test_pawns : white captures black - " );
    EXPECT(is_valid_move(4,6,3,5) == true); // diagonal capture
    EXPECT(is_valid_move(4,6,5,5) == true); // diagonal capture


    // wrong-diagonal when empty
    set_piece(3,5,EMPTY,NO_PLAYER);
    set_piece(5,5,EMPTY,NO_PLAYER);
    print_board();
    printf("test_pawns : white gonally when space is empty - " );
    EXPECT(is_valid_move(4,6,3,5) == false);
    EXPECT(is_valid_move(4,6,5,5) == false);

    // wrong-diagonal when empty
    set_piece(3,5,KNIGHT,PLAYER_ONE);
    set_piece(5,5,QUEEN,PLAYER_ONE);
    print_board();
    printf("test_pawns : white can't capture white - " );
    EXPECT(is_valid_move(4,6,3,5) == false);
    EXPECT(is_valid_move(4,6,5,5) == false);

    // Black pawn at (4,1) - standard start (e2 in traditional coords if y=6 is 2nd rank)
    set_piece(4,1,PAWN,PLAYER_TWO);
    print_board();

    // single space 
    printf("test_pawns : move black single - " );
    EXPECT(is_valid_move(4,1,4,2) == true);
    printf("test_pawns : move black double - " );
    EXPECT(is_valid_move(4,1,4,3) == true);

    // block forward
    printf("test_pawns : block black with black - " );
    set_piece(4,2,PAWN,PLAYER_TWO); // own piece blocks
    print_board();
    EXPECT(is_valid_move(4,1,4,2) == false); // single
    EXPECT(is_valid_move(4,1,4,3) == false); // double

    printf("test_pawns : block black with white - " );
    set_piece(4,2,PAWN,PLAYER_ONE); 
    print_board();
    EXPECT(is_valid_move(4,1,4,2) == false); //single
    EXPECT(is_valid_move(4,1,4,3) == false); // double

    // clear block, add enemy at diagonal for capture
    set_piece(4,2,EMPTY,NO_PLAYER);
    set_piece(3,2,PAWN,PLAYER_ONE);
    set_piece(5,2,PAWN,PLAYER_ONE);
    print_board();
    printf("test_pawns : black captures white - " );
    EXPECT(is_valid_move(4,1,3,2) == true); // diagonal capture
    EXPECT(is_valid_move(4,1,5,2) == true); // diagonal capture


    // gonal when empty
    set_piece(3,2,EMPTY,NO_PLAYER);
    set_piece(5,2,EMPTY,NO_PLAYER);
    print_board();
    printf("test_pawns : black gonally when space is empty - " );
    EXPECT(is_valid_move(4,1,3,2) == false);
    EXPECT(is_valid_move(4,1,5,2) == false);

    // gonal when same player
    set_piece(3,2,KNIGHT,PLAYER_TWO);
    set_piece(5,2,QUEEN,PLAYER_TWO);
    print_board();
    printf("test_pawns : black can't capture black - " );
    EXPECT(is_valid_move(4,1,3,2) == false);
    EXPECT(is_valid_move(4,1,5,2) == false);



}



int main( int argc, char* argv[] ) {
    test_pawns();

    return 0;
}
