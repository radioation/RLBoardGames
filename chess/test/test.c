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

void test_knights() {
    printf("test_knights --------------------------------------------\n");
    clear_board();

    printf("test_knights : setup board\n " );

    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(4,0,KING,PLAYER_TWO);

    // Knight at d4-ish (3,4)
    set_piece(3,4,KNIGHT,PLAYER_ONE);
    print_board();

    printf("test_knights : standard L - " );
    EXPECT(is_valid_move(3,4,4,2) == true);
    EXPECT(is_valid_move(3,4,5,3) == true);
    EXPECT(is_valid_move(3,4,5,5) == true);
    EXPECT(is_valid_move(3,4,4,6) == true);
    EXPECT(is_valid_move(3,4,2,6) == true);
    EXPECT(is_valid_move(3,4,1,5) == true);
    EXPECT(is_valid_move(3,4,1,3) == true);
    EXPECT(is_valid_move(3,4,2,2) == true);
    
    // Friendly piece at one target
    set_piece(4,2,PAWN,PLAYER_ONE);
    // Enemy piece at another target
    set_piece(4,6,PAWN,PLAYER_TWO);
    print_board();
    printf("test_knights : white can't capture white - " );
    EXPECT(is_valid_move(3,4,4,2) == false); // friendly occupied
    printf("test_knights : white can capture black - " );
    EXPECT(is_valid_move(3,4,4,6) == true);  // capture enemy

    printf("test_knights : white must move with L shape - " );
    EXPECT(is_valid_move(3,4,3,6) == false);

    // Knight at d4-ish (3,4)
    set_piece(3,4,KNIGHT,PLAYER_TWO);
    set_piece(4,2,EMPTY,NO_PLAYER);
    set_piece(4,6,EMPTY,NO_PLAYER);
    print_board();

    printf("test_knights : standard L - " );
    EXPECT(is_valid_move(3,4,4,2) == true);
    EXPECT(is_valid_move(3,4,5,3) == true);
    EXPECT(is_valid_move(3,4,5,5) == true);
    EXPECT(is_valid_move(3,4,4,6) == true);
    EXPECT(is_valid_move(3,4,2,6) == true);
    EXPECT(is_valid_move(3,4,1,5) == true);
    EXPECT(is_valid_move(3,4,1,3) == true);
    EXPECT(is_valid_move(3,4,2,2) == true);

    set_piece(4,2,PAWN,PLAYER_TWO);
    set_piece(4,6,PAWN,PLAYER_ONE);
    print_board();
    printf("test_knights : black can't capture black - " );
    EXPECT(is_valid_move(3,4,4,2) == false); // friendly occupied
    printf("test_knights : black can capture white - " );
    EXPECT(is_valid_move(3,4,4,6) == true);  // capture enemy

    printf("test_knights : black must move with L shape - " );
    EXPECT(is_valid_move(3,4,3,6) == false);

}

void test_bishops() {
    printf("test_bishops --------------------------------------------\n");
    clear_board();
    printf("test_bishops : setup board\n " );

    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(4,0,KING,PLAYER_TWO);

    set_piece(3,4,BISHOP,PLAYER_ONE);
    print_board();

    // must be on diagonal
    printf("test_bishops : can't move vertical - " );
    EXPECT(is_valid_move(3,4,3,2) == false); 
    EXPECT(is_valid_move(3,4,3,6) == false);
    printf("test_bishops : can't move horizontal - " );
    EXPECT(is_valid_move(3,4,6,4) == false); 
    EXPECT(is_valid_move(3,4,1,4) == false);
    printf("test_bishops : can move diagonally - " );
    EXPECT(is_valid_move(3,4,1,2) == true); 
    EXPECT(is_valid_move(3,4,5,2) == true);
    EXPECT(is_valid_move(3,4,5,6) == true); 
    EXPECT(is_valid_move(3,4,1,6) == true);

    // can't jump over pieces
    set_piece(2,3,PAWN,PLAYER_ONE);
    set_piece(4,3,KNIGHT,PLAYER_ONE);
    set_piece(4,5,BISHOP,PLAYER_ONE);
    set_piece(2,5,ROOK,PLAYER_ONE);
    print_board();
    printf("test_bishops : can't jump over pieces - " );
    EXPECT(is_valid_move(3,4,1,2) == false); 
    EXPECT(is_valid_move(3,4,5,2) == false);
    EXPECT(is_valid_move(3,4,5,6) == false); 
    EXPECT(is_valid_move(3,4,1,6) == false);

    printf("test_bishops : can't capture same side- " );
    EXPECT(is_valid_move(3,4,2,3) == false); 
    EXPECT(is_valid_move(3,4,4,3) == false);
    EXPECT(is_valid_move(3,4,4,5) == false); 
    EXPECT(is_valid_move(3,4,2,5) == false);

    printf("test_bishops : can capture opposite side- " );
    set_piece(2,3,PAWN,PLAYER_TWO);
    set_piece(4,3,KNIGHT,PLAYER_TWO);
    set_piece(4,5,BISHOP,PLAYER_TWO);
    set_piece(2,5,ROOK,PLAYER_TWO);
    print_board();
    EXPECT(is_valid_move(3,4,2,3) == true); 
    EXPECT(is_valid_move(3,4,4,3) == true);
    EXPECT(is_valid_move(3,4,4,5) == true); 
    EXPECT(is_valid_move(3,4,2,5) == true);

}


void test_rooks() {
    printf("test_rooks --------------------------------------------\n");
    clear_board();
    printf("test_bishops : setup board\n " );

    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(4,0,KING,PLAYER_TWO);

    set_piece(3,4,ROOK,PLAYER_ONE);
    print_board();

    // must be on diagonal

    // can't jump over pieces


}



int main( int argc, char* argv[] ) {
    test_pawns();
    test_knights();
    test_bishops();

    return 0;
}
