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
    printf("test_rooks : setup board\n " );

    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(4,0,KING,PLAYER_TWO);

    set_piece(3,4,ROOK,PLAYER_ONE);
    print_board();

    // Must be vertical or horizontal
    // can't be on diagonal
    printf("test_rooks : can move vertical - " );
    EXPECT(is_valid_move(3,4,3,2) == true); 
    EXPECT(is_valid_move(3,4,3,6) == true);
    printf("test_rooks : can move horizontal - " );
    EXPECT(is_valid_move(3,4,6,4) == true); 
    EXPECT(is_valid_move(3,4,1,4) == true);
    printf("test_rooks : can't move diagonally - " );
    EXPECT(is_valid_move(3,4,1,2) == false); 
    EXPECT(is_valid_move(3,4,5,2) == false);
    EXPECT(is_valid_move(3,4,5,6) == false); 
    EXPECT(is_valid_move(3,4,1,6) == false);

    // can't jump over pieces
    set_piece(3,3,PAWN,PLAYER_ONE);
    set_piece(4,4,KNIGHT,PLAYER_ONE);
    set_piece(3,5,BISHOP,PLAYER_ONE);
    set_piece(2,4,ROOK,PLAYER_ONE);
    print_board();
    printf("test_rooks : can't jump over pieces - " );
    EXPECT(is_valid_move(3,4,3,2) == false); 
    EXPECT(is_valid_move(3,4,5,4) == false);
    EXPECT(is_valid_move(3,4,3,6) == false); 
    EXPECT(is_valid_move(3,4,1,4) == false);

    printf("test_rooks : can't capture same side- " );
    EXPECT(is_valid_move(3,4,3,3) == false); 
    EXPECT(is_valid_move(3,4,4,4) == false);
    EXPECT(is_valid_move(3,4,3,5) == false); 
    EXPECT(is_valid_move(3,4,2,4) == false);

    printf("test_rooks : can capture opposite side- " );
    set_piece(3,3,PAWN,PLAYER_TWO);
    set_piece(4,4,KNIGHT,PLAYER_TWO);
    set_piece(3,5,BISHOP,PLAYER_TWO);
    set_piece(2,4,ROOK,PLAYER_TWO);
    print_board();
    EXPECT(is_valid_move(3,4,3,3) == true); 
    EXPECT(is_valid_move(3,4,4,4) == true);
    EXPECT(is_valid_move(3,4,3,5) == true); 
    EXPECT(is_valid_move(3,4,2,4) == true);

}

void test_queens() {
    printf("test_queens --------------------------------------------\n");
    clear_board();
    printf("test_queens : setup board\n " );


    set_piece(3,4,QUEEN,PLAYER_ONE);
    print_board();
    // Can move vertical, horizontal, or diagonally
    printf("test_queens : can move vertical - " );
    EXPECT(is_valid_move(3,4,3,2) == true); 
    EXPECT(is_valid_move(3,4,3,6) == true);
    printf("test_queens : can move horizontal - " );
    EXPECT(is_valid_move(3,4,6,4) == true); 
    EXPECT(is_valid_move(3,4,1,4) == true);
    printf("test_quens : can move diagonally - " );
    EXPECT(is_valid_move(3,4,1,2) == true); 
    EXPECT(is_valid_move(3,4,5,2) == true);
    EXPECT(is_valid_move(3,4,5,6) == true); 
    EXPECT(is_valid_move(3,4,1,6) == true);

    printf("test_quens : odd moves - " );
    EXPECT(is_valid_move(3,4,6,3) == false); 
    EXPECT(is_valid_move(3,4,2,1) == false); 
    EXPECT(is_valid_move(3,4,5,0) == false); 
    EXPECT(is_valid_move(3,4,7,6) == false); 

    // can't jump over pieces
    set_piece(3,3,PAWN,PLAYER_TWO);
    set_piece(4,4,KNIGHT,PLAYER_TWO);
    set_piece(3,5,BISHOP,PLAYER_TWO);
    set_piece(2,4,ROOK,PLAYER_TWO);
    set_piece(2,3,PAWN,PLAYER_ONE);
    set_piece(4,3,KNIGHT,PLAYER_ONE);
    set_piece(4,5,BISHOP,PLAYER_ONE);
    set_piece(2,5,ROOK,PLAYER_ONE);
    print_board();
    printf("test_queens : can't jump - " );
    EXPECT(is_valid_move(3,4,3,2) == false); 
    EXPECT(is_valid_move(3,4,5,4) == false);
    EXPECT(is_valid_move(3,4,3,6) == false); 
    EXPECT(is_valid_move(3,4,1,4) == false);
    EXPECT(is_valid_move(3,4,1,2) == false); 
    EXPECT(is_valid_move(3,4,5,2) == false);
    EXPECT(is_valid_move(3,4,5,6) == false); 
    EXPECT(is_valid_move(3,4,1,6) == false);

    printf("test_queens : can capture opposite side- " );
    EXPECT(is_valid_move(3,4,3,3) == true); 
    EXPECT(is_valid_move(3,4,4,4) == true);
    EXPECT(is_valid_move(3,4,3,5) == true); 
    EXPECT(is_valid_move(3,4,2,4) == true);

    printf("test_queens : can't capture same side- " );
    EXPECT(is_valid_move(3,4,2,3) == false); 
    EXPECT(is_valid_move(3,4,4,3) == false);
    EXPECT(is_valid_move(3,4,4,5) == false); 
    EXPECT(is_valid_move(3,4,2,5) == false);
}

void test_kings() {
    printf("test_kings --------------------------------------------\n");
    clear_board();
    printf("test_kings : setup board\n " );
    set_piece(3,4,KING,PLAYER_ONE);
    print_board();
    // Can move vertical, horizontal, or diagonally
    printf("test_kings : can move one square " );
    EXPECT(is_valid_move(3,4,3,3) == true ); 
    EXPECT(is_valid_move(3,4,4,4) == true );
    EXPECT(is_valid_move(3,4,3,5) == true ); 
    EXPECT(is_valid_move(3,4,2,4) == true );
    EXPECT(is_valid_move(3,4,2,3) == true ); 
    EXPECT(is_valid_move(3,4,4,3) == true );
    EXPECT(is_valid_move(3,4,4,5) == true ); 
    EXPECT(is_valid_move(3,4,2,5) == true );

    printf("test_kings : can't move two squares " );
    EXPECT(is_valid_move(3,4,3,2) == false); 
    EXPECT(is_valid_move(3,4,5,4) == false);
    EXPECT(is_valid_move(3,4,3,6) == false); 
    EXPECT(is_valid_move(3,4,1,4) == false);
    EXPECT(is_valid_move(3,4,1,2) == false); 
    EXPECT(is_valid_move(3,4,5,2) == false);
    EXPECT(is_valid_move(3,4,5,6) == false); 
    EXPECT(is_valid_move(3,4,1,6) == false);


    set_piece(3,3,PAWN,PLAYER_TWO);
    set_piece(4,4,KNIGHT,PLAYER_TWO);
    set_piece(3,5,BISHOP,PLAYER_TWO);
    set_piece(2,4,ROOK,PLAYER_TWO);
    set_piece(2,3,PAWN,PLAYER_ONE);
    set_piece(4,3,KNIGHT,PLAYER_ONE);
    set_piece(4,5,BISHOP,PLAYER_ONE);
    set_piece(2,5,ROOK,PLAYER_ONE);
    print_board();
    printf("test_queens : can capture opposite side- " );
    EXPECT(is_valid_move(3,4,3,3) == true); 
    EXPECT(is_valid_move(3,4,4,4) == true);
    EXPECT(is_valid_move(3,4,3,5) == true); 
    EXPECT(is_valid_move(3,4,2,4) == true);

    printf("test_queens : can't capture same side- " );
    EXPECT(is_valid_move(3,4,2,3) == false); 
    EXPECT(is_valid_move(3,4,4,3) == false);
    EXPECT(is_valid_move(3,4,4,5) == false); 
    EXPECT(is_valid_move(3,4,2,5) == false);
}

int test_check() {
    printf("test_check --------------------------------------------\n");
    clear_board();
    printf("test_check : setup board\n " );
    set_piece(3,4,KING,PLAYER_ONE);
    print_board();


    printf("test_check : no threatening pieces" );
    EXPECT(is_my_king_in_check( board, PLAYER_ONE) == false );

    set_piece(2,3,PAWN,PLAYER_TWO);
    print_board();
    printf("test_check : black pawn " );
    EXPECT(is_my_king_in_check( board, PLAYER_ONE) == true );
    set_piece(2,3,EMPTY,NO_PLAYER);
    set_piece(4,3,PAWN,PLAYER_TWO);
    print_board();
    printf("test_check : black pawn v2 " );
    EXPECT(is_my_king_in_check( board, PLAYER_ONE) == true );

    set_piece(4,3,EMPTY,NO_PLAYER);
    set_piece(3,3,PAWN,PLAYER_TWO);
    print_board();
    printf("test_check : black pawn NOT ATTACKING " );
    EXPECT(is_my_king_in_check( board, PLAYER_ONE) == false );


    set_piece(3,3,EMPTY,NO_PLAYER);
    set_piece(4,3,PAWN,PLAYER_ONE);
    print_board();
    printf("test_check : white pawn at diagonal " );
    EXPECT(is_my_king_in_check( board, PLAYER_ONE) == false );

    set_piece(4,3,EMPTY,NO_PLAYER);
    set_piece(2,3,PAWN,PLAYER_ONE);
    print_board();
    printf("test_check : white pawn at diagonal 2 " );
    EXPECT(is_my_king_in_check( board, PLAYER_ONE) == false );


    // black king
    clear_board();
    set_piece(3,4,KING,PLAYER_TWO);
    set_piece(4,6,PAWN,PLAYER_ONE);
    print_board();


    printf("test_check : no threatening pieces" );
    EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );

    printf("test_check : white pawn " );
    set_piece(4,6,EMPTY,NO_PLAYER);
    set_piece(4,5,PAWN,PLAYER_ONE);
    print_board();
    EXPECT(is_my_king_in_check( board, PLAYER_TWO) == true );

    printf("test_check : white pawn 2" );
    set_piece(4,5,EMPTY,NO_PLAYER);
    set_piece(2,5,PAWN,PLAYER_ONE);
    print_board();
    EXPECT(is_my_king_in_check( board, PLAYER_TWO) == true );

    printf("test_check : white pawn NOT ATTACKING" );
    set_piece(2,5,EMPTY,NO_PLAYER);
    set_piece(3,5,PAWN,PLAYER_ONE);
    print_board();
    EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );

    set_piece(3,5,EMPTY,NO_PLAYER);
    set_piece(4,5,PAWN,PLAYER_TWO);
    print_board();
    printf("test_check : black pawn at diagonal " );
    EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );

    set_piece(4,5,EMPTY,NO_PLAYER);
    set_piece(2,5,PAWN,PLAYER_TWO);
    print_board();
    printf("test_check : black pawn at diagonal 2 " );
    EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );


    // knights (way more typing than I was hoping to do)
    set_piece(2,5,KNIGHT,PLAYER_ONE);
    print_board();
    printf("test_check : Non threatening knight " );
    EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );

    const s8 knight_pos[8][2] = {
        { 2,2 },
        { 4,2 },
        { 5,3 },
        { 5,5 },
        { 4,6 },
        { 2,6 },
        { 1,5 },
        { 1,3 },
    };

    set_piece(2,5,EMPTY,NO_PLAYER);
    for( s8 i=0; i < 8; i++ ) {
        set_piece(knight_pos[i][0],knight_pos[i][1],KNIGHT,PLAYER_ONE);
        print_board();
        printf("test_check : threatening knight %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == true );
        set_piece(knight_pos[i][0],knight_pos[i][1],EMPTY,NO_PLAYER);
    }

    // bishwop
    const s8 bishop_pos[4][2] = {
        { 1,2 },
        { 5,2 },
        { 5,6 },
        { 1,6 },
    };

    for( s8 i=0; i < 4; i++ ) {
        set_piece(bishop_pos[i][0],bishop_pos[i][1],BISHOP,PLAYER_ONE);
        print_board();
        printf("test_check : threatening bishop %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == true );
        set_piece(bishop_pos[i][0],bishop_pos[i][1],EMPTY,NO_PLAYER);
    }

    const s8 blocker_pos[4][2] = {
        { 2,3 },
        { 4,3 },
        { 4,5 },
        { 2,5 },
    };
    for( s8 i=0; i < 4; i++ ) {
        set_piece(bishop_pos[i][0],bishop_pos[i][1],BISHOP,PLAYER_ONE);
        set_piece(blocker_pos[i][0],blocker_pos[i][1],PAWN,PLAYER_TWO);
        print_board();
        printf("test_check : blocked bishop %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );
        set_piece(bishop_pos[i][0],bishop_pos[i][1],EMPTY,NO_PLAYER);
        set_piece(blocker_pos[i][0],blocker_pos[i][1],EMPTY,NO_PLAYER);
    }

    //rook 
    const s8 rook_pos[4][2] = {
        { 3,2 },
        { 5,4 },
        { 3,6 },
        { 1,4 },
    };

    for( s8 i=0; i < 4; i++ ) {
        set_piece(rook_pos[i][0],rook_pos[i][1],ROOK,PLAYER_ONE);
        print_board();
        printf("test_check : threatening rook %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == true );
        set_piece(rook_pos[i][0],rook_pos[i][1],EMPTY,NO_PLAYER);
    }

    const s8 rook_blocker_pos[4][2] = {
        { 3,3 },
        { 4,4 },
        { 3,5 },
        { 2,4 },
    };
    for( s8 i=0; i < 4; i++ ) {
        set_piece(rook_pos[i][0],rook_pos[i][1],ROOK,PLAYER_ONE);
        set_piece(rook_blocker_pos[i][0],rook_blocker_pos[i][1],PAWN,PLAYER_TWO);
        print_board();
        printf("test_check : blocked rook %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );
        set_piece(rook_pos[i][0],rook_pos[i][1],EMPTY,NO_PLAYER);
        set_piece(rook_blocker_pos[i][0],rook_blocker_pos[i][1],EMPTY,NO_PLAYER);
    }

    // queen

    for( s8 i=0; i < 4; i++ ) {
        set_piece(rook_pos[i][0],rook_pos[i][1],QUEEN,PLAYER_ONE);
        print_board();
        printf("test_check : threatening queen %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == true );
        set_piece(rook_pos[i][0],rook_pos[i][1],EMPTY,NO_PLAYER);
    }
    for( s8 i=0; i < 4; i++ ) {
        set_piece(bishop_pos[i][0],bishop_pos[i][1],QUEEN,PLAYER_ONE);
        print_board();
        printf("test_check : threatening queen -diag %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == true );
        set_piece(bishop_pos[i][0],bishop_pos[i][1],EMPTY,NO_PLAYER);
    }


    for( s8 i=0; i < 4; i++ ) {
        set_piece(rook_pos[i][0],rook_pos[i][1],QUEEN,PLAYER_ONE);
        set_piece(rook_blocker_pos[i][0],rook_blocker_pos[i][1],PAWN,PLAYER_TWO);
        print_board();
        printf("test_check : blocked queen %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );
        set_piece(rook_pos[i][0],rook_pos[i][1],EMPTY,NO_PLAYER);
        set_piece(rook_blocker_pos[i][0],rook_blocker_pos[i][1],EMPTY,NO_PLAYER);
    }
    for( s8 i=0; i < 4; i++ ) {
        set_piece(bishop_pos[i][0],bishop_pos[i][1],QUEEN,PLAYER_ONE);
        set_piece(blocker_pos[i][0],blocker_pos[i][1],PAWN,PLAYER_TWO);
        print_board();
        printf("test_check : blocked bishop -diag %d ", i );
        EXPECT(is_my_king_in_check( board, PLAYER_TWO) == false );
        set_piece(bishop_pos[i][0],bishop_pos[i][1],EMPTY,NO_PLAYER);
        set_piece(blocker_pos[i][0],blocker_pos[i][1],EMPTY,NO_PLAYER);
    }





}


void test_find_king() {
    printf("test_find_king ----------------------------------------\n");
    clear_board();
    printf("test_find_king : setup board\n " );
    set_piece(5,2,KING,PLAYER_ONE);
    print_board();
    s8 kings_x = 0; 
    s8 kings_y = 0; 
    printf("test_find_king : king in middle\n " );
    bool ret = find_king( board, PLAYER_ONE, &kings_x, &kings_y );
    EXPECT( kings_x == 5 );
    EXPECT( kings_y == 2 );

    set_piece(5,2,EMPTY,NO_PLAYER);
    set_piece(0,0,KING,PLAYER_ONE);
    print_board();
    printf("test_find_king : king upper left\n " );
    find_king( board, PLAYER_ONE, &kings_x, &kings_y );
    EXPECT( kings_x == 0 );
    EXPECT( kings_y == 0 );

    printf("test_find_king : king lower right\n " );
    set_piece(0,0,EMPTY,NO_PLAYER);
    set_piece(7,7,KING,PLAYER_ONE);
    print_board();
    find_king( board, PLAYER_ONE, &kings_x, &kings_y );
    EXPECT( kings_x == 7 );
    EXPECT( kings_y == 7 );
    
}

int test_find_checkers() {
    printf("test_find_checkers --------------------------\n");
    clear_board();
    printf("test_find_checkers : setup board\n " );
    set_piece(3,4,KING,PLAYER_ONE);
    print_board();

    printf("test_find_checkers : no threatening pieces" );
    CHECKERS chkr = find_checkers( PLAYER_ONE, 3, 4);
    EXPECT( chkr.count == 0 );

    set_piece(2,3,PAWN,PLAYER_TWO);
    print_board();
    printf("test_find_checkers : one threatening piece" );
    chkr = find_checkers( PLAYER_ONE, 3, 4);
    EXPECT( chkr.count == 1 );

    set_piece(4,3,PAWN,PLAYER_TWO);
    print_board();
    printf("test_find_checkers : two threatening pieces" );
    chkr = find_checkers( PLAYER_ONE, 3, 4);
    EXPECT( chkr.count == 2 );


    clear_board();
    set_piece(3,4,KING,PLAYER_TWO);
    set_piece(4,6,PAWN,PLAYER_ONE);
    print_board();
    printf("test_check : no threatening pieces" );
    chkr = find_checkers( PLAYER_TWO, 3, 4);
    printf("chkr.count = %d", chkr.count );
    EXPECT( chkr.count == 0 );

    set_piece(4,6,EMPTY,NO_PLAYER);
    set_piece(4,5,PAWN,PLAYER_ONE);
    print_board();
    printf("test_find_checkers : one threatening piece" );
    chkr = find_checkers( PLAYER_TWO, 3, 4);
    EXPECT( chkr.count == 1 );

    set_piece(2,5,PAWN,PLAYER_ONE);
    print_board();
    printf("test_find_checkers : two threatening pieces" );
    chkr = find_checkers( PLAYER_TWO, 3, 4);
    EXPECT( chkr.count == 2 );

    // knights 
    set_piece(4,5,EMPTY,NO_PLAYER);
    set_piece(2,5,EMPTY,NO_PLAYER);
    set_piece(2,5,KNIGHT,PLAYER_ONE);
    print_board();
    printf("test_find_checkers : Non threatening knight " );
    chkr = find_checkers( PLAYER_TWO, 3, 4);
    EXPECT( chkr.count == 0 );

    const s8 knight_pos[8][2] = {
        { 2,2 },
        { 4,2 },
        { 5,3 },
        { 5,5 },
        { 4,6 },
        { 2,6 },
        { 1,5 },
        { 1,3 },
    };

    set_piece(2,5,EMPTY,NO_PLAYER);
    for( s8 i=0; i < 8; i = i+2 ) {
        set_piece(knight_pos[i][0],knight_pos[i][1],KNIGHT,PLAYER_ONE);
        set_piece(knight_pos[i+1][0],knight_pos[i+1][1],KNIGHT,PLAYER_ONE);
        print_board();
        printf("test_checkers : threatening knightS %d ", i );
        chkr = find_checkers( PLAYER_TWO, 3, 4);
        EXPECT( chkr.count == 2 );
        set_piece(knight_pos[i][0],knight_pos[i][1],EMPTY,NO_PLAYER);
        set_piece(knight_pos[i+1][0],knight_pos[i+1][1],EMPTY,NO_PLAYER);
    }

    const s8 bishop_pos[4][2] = {
        { 1,2 },
        { 5,2 },
        { 5,6 },
        { 1,6 },
    };

    for( s8 i=0; i < 4; i++ ) {
        set_piece(bishop_pos[i][0],bishop_pos[i][1],BISHOP,PLAYER_ONE);
        print_board();
        printf("test_checkers : threatening bishop(s) %d ", i+1 );
        chkr = find_checkers( PLAYER_TWO, 3, 4);
        s8 expected = i+1;
        if( expected > 2 ) expected = 2;
        EXPECT( chkr.count == expected );
    }

    //rook 
    const s8 rook_pos[4][2] = {
        { 3,2 },
        { 5,4 },
        { 3,6 },
        { 1,4 },
    };

    clear_board();
    set_piece(3,4,KING,PLAYER_ONE);
    for( s8 i=0; i < 4; i++ ) {
        set_piece(rook_pos[i][0],rook_pos[i][1],ROOK,PLAYER_TWO);
        print_board();
        printf("test_checkers : threatening rook %d ", i );
        chkr = find_checkers( PLAYER_ONE, 3, 4);
        s8 expected = i+1;
        if( expected > 2 ) expected = 2;
        EXPECT( chkr.count == expected );
    }

    // harmless
    clear_board();
    set_piece(3,4,KING,PLAYER_ONE);
    set_piece(3,2, ROOK, PLAYER_ONE);
    set_piece(1,2,BISHOP,PLAYER_ONE);
    print_board();
    printf("test_checkers : non threatening rook  and bishop " );
    chkr = find_checkers( PLAYER_ONE, 3, 4);
    EXPECT( chkr.count == 0 );

    set_piece(3,4,KING,PLAYER_TWO);
    print_board();
    printf("test_checkers :  threatening rook  and bishop " );
    chkr = find_checkers( PLAYER_TWO, 3, 4);
    EXPECT( chkr.count == 2 );

    set_piece(3,4,KING,PLAYER_TWO);
    set_piece(3,3, PAWN, PLAYER_TWO);
    print_board();
    printf("test_checkers :  blocked rook and threatening bishop " );
    chkr = find_checkers( PLAYER_TWO, 3, 4);
    EXPECT( chkr.count == 1 );


    set_piece(3,2,EMPTY,NO_PLAYER);
    set_piece(1,2,EMPTY,NO_PLAYER);  
    set_piece(4,3, PAWN, PLAYER_TWO);
    set_piece(4,4, PAWN, PLAYER_TWO);
    print_board();


    // queen
    for( s8 i=0; i < 4; i++ ) {
        set_piece(rook_pos[i][0],rook_pos[i][1],QUEEN,PLAYER_ONE);
        print_board();
        chkr = find_checkers( PLAYER_TWO, 3, 4);
        s8 expected = i < 2 ? 0 : 1;
        printf("test_checkers : threatening queen %d : expect: %d ", i, expected );
        EXPECT( chkr.count == expected );
        set_piece(rook_pos[i][0],rook_pos[i][1],EMPTY,NO_PLAYER);
    }
    for( s8 i=0; i < 4; i++ ) {
        set_piece(bishop_pos[i][0],bishop_pos[i][1],QUEEN,PLAYER_ONE);
        print_board();
        chkr = find_checkers( PLAYER_TWO, 3, 4);
        s8 expected = i == 1 ? 0 : 1;
        printf("test_checkers : threatening queen %d : expect: %d ", i, expected );
        EXPECT( chkr.count == expected );
        set_piece(bishop_pos[i][0],bishop_pos[i][1],EMPTY,NO_PLAYER);
    }


}




int test_block_square() {
    printf("test_block_square -------------------------------------\n");
    clear_board();
    s8 kings_x = 4;
    s8 kings_y = 4;
    s8 checker_x = 4;
    s8 checker_y = 0;
    printf("king (4,4) checker is rook (or queen) at 4,0\n");
    printf("* Blocking squares -");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,4,3) == true);
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,4,2) == true);
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,4,1) == true);
    printf("* rook square - ");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,4,0) == false); // checker square itself
    printf("* past king ( not inline ) - ");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,4,5) == false); // behind king
    printf("* not on vertical - ");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,3,3) == false); // off vertical 
    
    checker_x = 0;
    checker_y = 4;
    printf("king (4,4) checker is rook (or queen) at 0,4\n");
    printf("* Blocking squares -");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,3,4) == true);
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,2,4) == true);
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,1,4) == true);
    printf("* rook square - ");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,0,4) == false); // checker/ROOK
    printf("* past king ( not inline ) - ");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,5,4) == false); // behind king
    printf("* not on horizontal - ");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,3,3) == false); // off horizontal

    checker_x = 7;
    checker_y = 7;
    printf("king (4,4) checker is bishop (or queen) at 7,7\n");
    printf("* Blocking squares -");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,5,5) == true);
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,6,6) == true);
    printf("* bishop square - ");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,7,7) == false); // checker
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,3,3) == false); // past king

    checker_x = 1;
    checker_y = 7;
    printf("king (4,4) checker is bishop (or queen) at 1,7\n");
    printf("* Blocking squares -");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,3,5) == true);
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,2,6) == true);
    printf("* bishop square - ");
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,1,7) == false); // checker
    EXPECT(is_block_square(kings_x,kings_y,checker_x,checker_y,5,3) == false); // past-king



}


void test_has_any_valid_move() {
    printf("test_has_any_valid_move -------------------------------\n");
    clear_board();


    // Not in check : any legal move exists
    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(4,0,KING,PLAYER_TWO);
    set_piece(4,6,PAWN,PLAYER_ONE); // white pawn can move to (4,5)
    print_board();
    EXPECT(is_my_king_in_check( board, PLAYER_ONE) == false );
    EXPECT(has_any_valid_move(PLAYER_ONE) == true);


    clear_board();
    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(4,0,KING,PLAYER_TWO);
    set_piece(5,5,KNIGHT,PLAYER_TWO); // checking knight
    set_piece(6,6,PAWN,PLAYER_ONE);   // can capture up-left to (5,5)
    print_board();
    EXPECT(is_my_king_in_check(board, PLAYER_ONE) == true);
    EXPECT(has_any_valid_move(PLAYER_ONE) == true);



    clear_board();
    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(0,0,KING,PLAYER_TWO);

    set_piece(1,4,BISHOP,PLAYER_TWO); // checking along (1,4)->(4,7)
                                      // Empty the line (2,5) and (3,6)
                                      // White rook that can drop to (2,5) (ensure (2,6) empty)
    set_piece(2,7,ROOK,PLAYER_ONE);
    print_board();
    EXPECT(is_my_king_in_check( board, PLAYER_ONE) == true); // bishop
    EXPECT(has_any_valid_move( PLAYER_ONE) == true); // rook can block 


    clear_board();
    set_piece(4,4,KING,PLAYER_ONE);
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(4,0,ROOK,PLAYER_TWO);   // check on file
    set_piece(7,7,BISHOP,PLAYER_TWO); // check on diagonal to (4,4)
    print_board();                    // Make sure (3,4) is empty and not attacked by other pieces
    EXPECT(is_my_king_in_check(board, PLAYER_ONE) == true);
    EXPECT(has_any_valid_move(PLAYER_ONE) == true); // king step should exist


    clear_board();
    set_piece(4,4,KING,PLAYER_ONE);
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(4,0,ROOK,PLAYER_TWO);   
    set_piece(0,7,ROOK,PLAYER_ONE); // check 
    set_piece(1,3,ROOK,PLAYER_ONE); 
    print_board();                 
    EXPECT(is_my_king_in_check(board, PLAYER_TWO) == true);
    EXPECT(has_any_valid_move(PLAYER_TWO) == false); 


    clear_board();
    set_piece(6,7,KING,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(4,7,ROOK,PLAYER_TWO);   
    set_piece(6,4,KNIGHT,PLAYER_TWO); // check 
    print_board();                 
    EXPECT(is_my_king_in_check(board, PLAYER_ONE) == true);
    EXPECT(has_any_valid_move(PLAYER_ONE) == false); 


    clear_board();
    set_piece(6,7,KING,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(3,4,ROOK,PLAYER_ONE); // should NOT be able to block
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(4,7,ROOK,PLAYER_TWO);   
    set_piece(6,4,KNIGHT,PLAYER_TWO); // check 
    print_board();                 
    EXPECT(is_my_king_in_check(board, PLAYER_ONE) == true);
    EXPECT(has_any_valid_move(PLAYER_ONE) == false); 


    clear_board();
    set_piece(6,7,KING,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(3,4,ROOK,PLAYER_ONE); // should be able to block
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(1,7,ROOK,PLAYER_TWO);   
    set_piece(6,4,KNIGHT,PLAYER_TWO); // check 
    print_board();                 
    EXPECT(is_my_king_in_check(board, PLAYER_ONE) == true);
    EXPECT(has_any_valid_move(PLAYER_ONE) == true); 



    clear_board();
    set_piece(6,7,KING,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(4,4,BISHOP,PLAYER_ONE); // should be able to capture
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(1,7,ROOK,PLAYER_TWO);   
    set_piece(6,4,KNIGHT,PLAYER_TWO); // check 
    print_board();                 
    EXPECT(is_my_king_in_check(board, PLAYER_ONE) == true);
    EXPECT(has_any_valid_move(PLAYER_ONE) == true); 


    clear_board();
    set_piece(1,0,KING,PLAYER_TWO);
    set_piece(0,1,PAWN,PLAYER_TWO);
    set_piece(2,1,PAWN,PLAYER_TWO);
    set_piece(1,7,ROOK,PLAYER_TWO);   // should be able to attack queen
    set_piece(6,7,KING,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(5,7,KNIGHT,PLAYER_ONE);
    set_piece(4,4,BISHOP,PLAYER_ONE); 
    set_piece(1,1,QUEEN,PLAYER_ONE); 
    print_board();                 
    EXPECT(is_my_king_in_check(board, PLAYER_TWO) == true);
    EXPECT(has_any_valid_move(PLAYER_TWO) == true); 


    clear_board();
    set_piece(1,0,KING,PLAYER_TWO);
    set_piece(0,1,PAWN,PLAYER_TWO);
    set_piece(2,1,PAWN,PLAYER_TWO);
    set_piece(6,7,KING,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(5,7,KNIGHT,PLAYER_ONE);
    set_piece(4,4,BISHOP,PLAYER_ONE); 
    set_piece(1,1,QUEEN,PLAYER_ONE); 
    print_board();                 
    EXPECT(is_my_king_in_check(board, PLAYER_TWO) == true);
    EXPECT(has_any_valid_move(PLAYER_TWO) == false); 

}

bool test_any_valid_king_move() {
    printf("test_any_valid_king_move ------------------------------\n");

    clear_board();
    set_piece(4,4,KING,PLAYER_ONE);
    set_piece(0,0,KING,PLAYER_TWO);
    print_board();                 
    EXPECT(any_valid_king_move(PLAYER_TWO) == true ); 

    clear_board();
    set_piece(4,4,KING,PLAYER_ONE);
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(4,0,ROOK,PLAYER_TWO);   
    set_piece(0,7,ROOK,PLAYER_ONE); // check mate probably
    set_piece(1,3,ROOK,PLAYER_ONE); 
    print_board();                 
    EXPECT(any_valid_king_move(PLAYER_TWO) == false); 


    clear_board();
    set_piece(6,7,KING,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE); 
    set_piece(7,6,PAWN,PLAYER_ONE); 
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(4,5,BISHOP,PLAYER_TWO);  // check, but should be able to get out
    set_piece(5,1,ROOK,PLAYER_TWO);  
    print_board();                 
    EXPECT(any_valid_king_move(PLAYER_ONE) == true); 

    clear_board();
    set_piece(6,7,KING,PLAYER_ONE);
    set_piece(5,7,BISHOP,PLAYER_ONE);
    set_piece(7,7,ROOK,PLAYER_ONE);
    set_piece(5,6,PAWN,PLAYER_ONE); 
    set_piece(6,6,PAWN,PLAYER_ONE); 
    set_piece(7,6,PAWN,PLAYER_ONE); 
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(4,5,BISHOP,PLAYER_TWO);  // check, but should be able to get out
    set_piece(5,1,ROOK,PLAYER_TWO);  
    print_board();                 
    EXPECT(any_valid_king_move(PLAYER_ONE) == false); 



}

int test_checkmate() {
    printf("test_checkmate ----------------------------------------\n");
    clear_board();
    printf("test_checkmate : setup board\n " );


    clear_board();
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(1,1,QUEEN,PLAYER_ONE);
    set_piece(2,2,KING,PLAYER_ONE);
    print_board();
    printf("test_checkmate : king and queen\n " );
    EXPECT(is_my_king_in_check(board,PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == true);
    EXPECT(is_stalemate(PLAYER_TWO) == false);


    clear_board();
    set_piece(7,1,KING,PLAYER_TWO);
    set_piece(6,1,QUEEN,PLAYER_ONE);
    set_piece(5,2,KING,PLAYER_ONE);
    print_board();
    printf("test_checkmate : king and queen 2\n " );
    EXPECT(is_my_king_in_check(board,PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == true);
    EXPECT(is_stalemate(PLAYER_TWO) == false);

    clear_board();
    set_piece(1,0,KING,PLAYER_TWO);
    set_piece(7,0,ROOK,PLAYER_ONE);
    set_piece(1,2,KING,PLAYER_ONE);
    print_board();
    printf("test_checkmate : king and rook\n " );
    EXPECT(is_my_king_in_check(board,PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == true);
    EXPECT(is_stalemate(PLAYER_TWO) == false);

    clear_board();
    set_piece(7,0,KING,PLAYER_TWO);
    set_piece(4,2,BISHOP,PLAYER_ONE);
    set_piece(5,2,BISHOP,PLAYER_ONE);
    set_piece(7,2,KING,PLAYER_ONE);
    print_board();
    printf("test_checkmate : two bishops one king\n " );
    EXPECT(is_my_king_in_check(board,PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == true);
    EXPECT(is_stalemate(PLAYER_TWO) == false);

    clear_board();
    set_piece(0,0,ROOK,PLAYER_TWO);
    set_piece(1,0,KNIGHT,PLAYER_TWO);
    set_piece(2,0,BISHOP,PLAYER_TWO);
    set_piece(3,0,KING,PLAYER_TWO);
    set_piece(4,0,QUEEN,PLAYER_TWO);
    set_piece(5,0,BISHOP,PLAYER_TWO);
    set_piece(6,0,KNIGHT,PLAYER_TWO);
    set_piece(7,0,ROOK,PLAYER_TWO);

    set_piece(0,1,PAWN,PLAYER_TWO);
    set_piece(1,3,PAWN,PLAYER_TWO);
    set_piece(2,2,PAWN,PLAYER_TWO);
    set_piece(3,1,PAWN,PLAYER_TWO);
    set_piece(4,1,PAWN,PLAYER_TWO);
    set_piece(5,1,PAWN,PLAYER_TWO);
    set_piece(6,1,PAWN,PLAYER_TWO);
    set_piece(7,1,PAWN,PLAYER_TWO);

    set_piece(0,3,QUEEN,PLAYER_ONE);

    set_piece(0,6,PAWN,PLAYER_ONE);
    set_piece(1,6,PAWN,PLAYER_ONE);
    set_piece(2,6,PAWN,PLAYER_ONE);
    set_piece(3,5,PAWN,PLAYER_ONE);
    set_piece(4,6,PAWN,PLAYER_ONE);
    set_piece(5,6,PAWN,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(7,6,PAWN,PLAYER_ONE);
    set_piece(0,7,ROOK,PLAYER_ONE);
    set_piece(1,7,KNIGHT,PLAYER_ONE);
    set_piece(2,7,BISHOP,PLAYER_ONE);
    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(5,7,BISHOP,PLAYER_ONE);
    set_piece(6,7,KNIGHT,PLAYER_ONE);
    set_piece(7,7,ROOK,PLAYER_ONE);
    print_board();
    printf("test_checkmate : Fool's mate\n " );
    EXPECT(is_my_king_in_check(board,PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == true);
    EXPECT(is_stalemate(PLAYER_TWO) == false);


    clear_board();
    set_piece(0,0,ROOK,PLAYER_TWO);
    set_piece(2,2,KNIGHT,PLAYER_TWO);
    set_piece(2,0,BISHOP,PLAYER_TWO);
    set_piece(3,0,QUEEN,PLAYER_TWO);
    set_piece(4,0,KING,PLAYER_TWO);
    set_piece(2,3,BISHOP,PLAYER_TWO);
    set_piece(6,0,KNIGHT,PLAYER_TWO);
    set_piece(7,0,ROOK,PLAYER_TWO);
    set_piece(0,1,PAWN,PLAYER_TWO);
    set_piece(1,1,PAWN,PLAYER_TWO);
    set_piece(2,1,PAWN,PLAYER_TWO);
    set_piece(3,1,PAWN,PLAYER_TWO);
    set_piece(4,3,PAWN,PLAYER_TWO);
    
    set_piece(6,1,PAWN,PLAYER_TWO);
    set_piece(7,1,PAWN,PLAYER_TWO);

    set_piece(0,6,PAWN,PLAYER_ONE);
    set_piece(1,6,PAWN,PLAYER_ONE);
    set_piece(2,6,PAWN,PLAYER_ONE);
    set_piece(3,6,PAWN,PLAYER_ONE);
    set_piece(4,4,PAWN,PLAYER_ONE);
    set_piece(5,6,PAWN,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(7,6,PAWN,PLAYER_ONE);
    set_piece(0,7,ROOK,PLAYER_ONE);
    set_piece(1,7,KNIGHT,PLAYER_ONE);
    set_piece(2,7,BISHOP,PLAYER_ONE);
    set_piece(5,1,QUEEN,PLAYER_ONE);
    set_piece(4,7,KING,PLAYER_ONE);
    set_piece(2,4,BISHOP,PLAYER_ONE);
    set_piece(6,7,KNIGHT,PLAYER_ONE);
    set_piece(7,7,ROOK,PLAYER_ONE);
    print_board();
    printf("test_checkmate : Four move mate\n " );
    EXPECT(is_my_king_in_check(board,PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == true);
    EXPECT(is_stalemate(PLAYER_TWO) == false);


    clear_board();
    set_piece(6,0,KING,PLAYER_TWO);
    set_piece(5,1,PAWN,PLAYER_TWO);
    set_piece(6,1,PAWN,PLAYER_TWO);
    set_piece(7,1,PAWN,PLAYER_TWO);

    set_piece(4,0,ROOK,PLAYER_ONE);
    set_piece(5,6,PAWN,PLAYER_ONE);
    set_piece(6,6,PAWN,PLAYER_ONE);
    set_piece(7,6,PAWN,PLAYER_ONE);
    set_piece(6,7,KING,PLAYER_ONE);
    print_board();
    printf("test_checkmate : Back Rank mate\n " );
    EXPECT(is_my_king_in_check(board,PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == true);
    EXPECT(is_stalemate(PLAYER_TWO) == false);



}

int test_stalemate() {
    printf("test_stalemate ----------------------------------------\n");
    clear_board();
    printf("test_stalemate : setup board\n " );


    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(1,2,QUEEN,PLAYER_ONE);
    set_piece(2,1,KING,PLAYER_ONE);
    print_board();
    printf("test_stalemate : King and Queen\n " );
    EXPECT(is_my_king_in_check(board, PLAYER_TWO) == false);
    EXPECT(is_stalemate(PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == false);

    clear_board();
    set_piece(7,7,KING,PLAYER_TWO);
    set_piece(5,6,QUEEN,PLAYER_ONE);
    set_piece(0,0,KING,PLAYER_ONE);
    print_board();
    printf("test_stalemate : King and Queen 2\n " );
    EXPECT(is_my_king_in_check(board, PLAYER_TWO) == false);
    EXPECT(is_stalemate(PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == false);


    clear_board();
    set_piece(0,0,KING,PLAYER_TWO);
    set_piece(0,4,KNIGHT,PLAYER_TWO);
    set_piece(3,3,PAWN,PLAYER_TWO);

    set_piece(3,4,PAWN,PLAYER_ONE);
    set_piece(0,7,ROOK,PLAYER_ONE);
    set_piece(7,1,ROOK,PLAYER_ONE);
    set_piece(1,7,QUEEN,PLAYER_ONE);
    set_piece(7,7,KING,PLAYER_ONE);
    print_board();
    printf("test_stalemate : Pinned Knight\n " );
    EXPECT(is_my_king_in_check(board, PLAYER_TWO) == false);
    EXPECT(is_stalemate(PLAYER_TWO) == true);
    EXPECT(is_checkmate(PLAYER_TWO) == false);

}


int main( int argc, char* argv[] ) {
    test_pawns();
    test_knights();
    test_bishops();
    test_rooks();
    test_queens();
    test_kings();

    test_check();
    test_find_king();

    test_find_checkers();
    test_block_square();

    test_has_any_valid_move();

    test_any_valid_king_move();
    test_checkmate();

    test_stalemate();

    return 0;
}
