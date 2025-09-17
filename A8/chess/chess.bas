' SETUP MEMORY FOR CUSTOM CHARS
' MOVE MEMORY BY 4 PAGES
NMEMTOP=PEEK(106)-4
POKE 106,NMEMTOP

CHROM=PEEK(756)*256 ' FIND LOCATION OF

'DLI SET d1 = 144 WSYNC INTO $D018,
DLI SET d1 = 144 INTO $D018,
DLI        = $8A INTO $D017
GRAPHICS 12 + 16        ' ANTIC MODE 4

dl = peek( 560) + peek( 561)* 256 + 4
poke dl-1, $46
poke dl+2, $06
mset dl+3, 19, $04
poke dl+22, $02 + 130
poke dl+23, $02
poke dl+24, $02
poke dl+25, $41
poke dl+26, peek(560)
poke dl+27, peek(561)
'POKE DPEEK(560) + 25, 130
DLI d1


'DATA DL() byte = 70,70,70,46, 00,00, 06,04,04,04,
'DATA byte =  04,04,04,04,04,04,04,04,04,04,
'DATA byte =  04,04,04,04,04,04,04,04,04,41,
'DATA byte =  00,00
'
'dpoke Adr(DL) + 30, ADR(DL)
' value = (hue * 16) + luminance.

CHRAM=NMEMTOP*256 ' SET A LOCATION FOR

'SET THE COLORS
IF PEEK($D014) = 1
' PAL
SE. 0,3,12
SE. 1,6,12
SE. 2,9,6
SE. 3,8,6
ELSE
' NTSC
SE. 0,1,8     ' yellow?
SE. 1,12,2    ' green 
SE. 2,0,0     ' black
SE. 3,0,14    ' white inverse.
SE. 4,9,0
ENDIF


' New font
' Size: 1024 bytes
data PIECES() byte = 0,0,0,0,0,0,0,0,
data byte = 85,85,85,85,85,85,85,85,          ' $21 $A1 : 01 : light square 
data byte = 170,170,170,170,170,170,170,170,  ' $22 $A2 : 02 : dark square
data byte = 255,255,125,125,125,255,255,85,   ' $23 $A3 : 03 : pawn bottom light sauare
data byte = 85,85,85,85,85,85,125,255,        ' $24 $A4 : 04 : paw top light square
data byte = 255,255,190,190,190,255,255,170,  ' $25 $A5 : 05 : pawn bottom dark square
data byte = 170,170,170,170,170,170,190,255,  ' $26 $A6 : 06 : pawn top dark square
data byte = 85,85,85,85,85,87,87,85,          ' $27 $A7 : 07 : QKN bottom 1 light
data byte = 255,127,127,127,255,255,255,85,   ' $28 $A8 : 08 : QKN bottom 2 light
data byte = 213,85,85,85,213,245,245,85,      ' $29 $A9 : 09 : QKN BOTTOM 3 light
data byte = 85,85,93,87,87,85,85,85,          ' $2A $AA : 10 : Queen Top 1 light
data byte = 85,93,221,255,255,255,255,127,    ' $2B $AB : 11 : Queen Top 2 light
data byte = 85,85,221,245,245,213,213,85,     ' $2C $AC : 12 : Queen Top 3 light
data byte = 170,170,170,170,170,171,171,170,  ' $2D $AD : 13 : QKN bottom 1 dark 
data byte = 255,191,191,191,255,255,255,170,  ' $2E $AE : 14 : QKN bottom 2 dark 
data byte = 234,170,170,170,234,250,250,170,  ' $2F $AF : 15 : QKN bottom 3 dark 
data byte = 0,60,102,110,118,102,60,0,
data byte = 0,24,56,24,24,24,126,0,
data byte = 0,60,102,12,24,48,126,0,
data byte = 0,126,12,24,12,102,60,0,
data byte = 0,12,28,60,108,126,12,0,
data byte = 0,126,96,124,6,102,60,0,
data byte = 0,60,96,124,102,102,60,0,
data byte = 0,126,6,12,24,48,48,0,
data byte = 0,60,102,60,102,102,60,0,
data byte = 0,60,102,62,6,12,56,0,
data byte = 0,0,24,24,0,24,24,0,
data byte = 0,0,24,24,0,24,24,48,
data byte = 6,12,24,48,24,12,6,0,
data byte = 0,0,126,0,0,126,0,0,
data byte = 96,48,24,12,24,48,96,0,
data byte = 0,60,102,12,24,0,24,0,
data byte = 0,60,102,110,110,96,62,0,
data byte = 0,24,60,102,102,126,102,0,
data byte = 0,124,102,124,102,102,124,0,
data byte = 0,60,102,96,96,102,60,0,
data byte = 0,120,108,102,102,108,120,0,
data byte = 0,126,96,124,96,96,126,0,
data byte = 0,126,96,124,96,96,96,0,
data byte = 0,62,96,96,110,102,62,0,
data byte = 0,102,102,126,102,102,102,0,
data byte = 0,126,24,24,24,24,126,0,
data byte = 0,6,6,6,6,102,60,0,
data byte = 0,102,108,120,120,108,102,0,
data byte = 0,96,96,96,96,96,126,0,
data byte = 0,99,119,127,107,99,99,0,
data byte = 0,102,118,126,126,110,102,0,
data byte = 0,60,102,102,102,102,60,0,
data byte = 0,124,102,102,124,96,96,0,
data byte = 0,60,102,102,102,108,54,0,
data byte = 0,124,102,102,124,108,102,0,
data byte = 0,60,96,60,6,6,60,0,
data byte = 0,126,24,24,24,24,24,0,
data byte = 0,102,102,102,102,102,126,0,
data byte = 0,102,102,102,102,60,24,0,
data byte = 0,99,99,107,127,119,99,0,
data byte = 0,102,102,60,60,102,102,0,
data byte = 0,102,102,60,24,24,24,0,
data byte = 0,126,12,24,48,96,126,0,
data byte = 170,170,174,171,171,170,170,170,     ' $5B $DB : 59 : Queen top 1 dark
data byte = 170,174,238,255,255,255,255,191,     ' $5C $DC : 60 : Queen top 2 dark
data byte = 170,170,238,250,250,234,234,170,     ' $5D $DD : 61 : Queen top 3 dark
data byte = 85,85,85,85,85,87,87,85,             ' $5E $DE : 62 : King top 1 light
data byte = 85,93,127,93,255,255,255,127,        ' $5F $DF : 63 : King top 2 light
data byte = 85,85,85,85,213,245,245,85,          ' $00 $80 : 64 : King top 3 light
data byte = 170,170,170,170,170,171,171,170,     ' $01 $81 : 65 : King top 1 dark
data byte = 170,174,191,174,255,255,255,191,     ' $02 $82 : 66 : King top 2 dark
data byte = 170,170,170,170,234,250,250,170,     ' $03 $83 : 67 : King top 3 dark
data byte = 85,85,221,221,255,255,255,255,       ' $04 $84 : 68 : Rook top 2 light
data byte = 85,85,213,213,213,213,213,213,       ' $05 $85 : 69 : Rook top 3 light
data byte = 170,170,238,238,255,255,255,255,     ' $06 $86 : 70 : Rook top 2 dark
data byte = 170,170,234,234,234,234,234,234,     ' $07 $87 : 71 : Rook top 3 dark
data byte = 87,85,85,85,85,85,87,85,             ' $08 $88 : 72 : Bishop Bottom 1 light
data byte = 255,255,127,127,255,255,255,85,      ' $09 $89 : 73 : Bishop/Knight Bottom 2 light
data byte = 245,213,85,85,213,213,245,85,        ' $0A $8A : 74 : Bishop Bottom 3 light
data byte = 85,85,85,85,85,85,85,87,             ' $0B $8B : 75 : Bishop top 1 light  
data byte = 85,85,93,93,127,95,215,247,          ' $0C $8C : 76 : Bishop top 2 light
data byte = 85,85,85,85,85,213,213,245,          ' $0D $8D : 77 : Bishop top 3 light
data byte = 171,170,170,170,170,170,171,170,     ' $0E $8E : 78 : Bishop Bottom 1 dark
data byte = 255,255,191,191,255,255,255,170,     ' $0F $8F : 79 : Bishop/Knight Bottom 2 dark
data byte = 250,234,170,170,234,234,250,170,     ' $10 $90 : 80 : Bishop Bottom 3 dark 
data byte = 170,170,170,170,170,170,170,171,     ' $11 $91 : 81 : Bishop top 1 dark
data byte = 170,170,174,174,191,175,235,251,     ' $12 $92 : 82 : Bishop top 2 dark
data byte = 170,170,170,170,170,234,234,250,     ' $13 $93 : 83 : Bishop top 3 dark
data byte = 95,95,87,85,85,87,87,85,             ' $14 $94 : 84 : Knight bottom 1 light 
data byte = 245,245,245,213,213,245,245,85,      ' $15 $95 : 85 : Knight bottom 3 light
data byte = 85,85,85,85,85,85,87,95,             ' $16 $96 : 86 : knight top 1 light
data byte = 85,85,125,255,223,255,255,255,       ' $17 $97 : 86 : knight top 2 light
data byte = 85,85,85,85,213,213,245,245,         ' $18 $98 : 86 : knight top 3 light
data byte = 175,175,171,170,170,171,171,170,     ' $19 $99 : 84 : Knight bottom 1 dark
data byte = 250,250,250,234,234,250,250,170,     ' $1A $9A : 85 : Knight bottom 3 dark

data byte = 170,170,170,170,170,170,171,175,     ' $1B $9B : 86 : knight top 1 dark
data byte = 170,170,190,255,239,255,255,255,     ' $1C $9C : 87 : knight top 2 dark
data byte = 170,170,170,170,234,234,250,250,     ' $1D $9D : 88 : knight top 3 dark
data byte = 170,170,170,170,170,170,171,175,     ' $1E $9E : 86 : knight top 1 dark
'data byte = 0,24,48,126,48,24,0,0,                
data byte = 0,24,12,126,12,24,0,0,
data byte = 0,24,60,126,126,60,24,0,
data byte = 0,0,60,6,62,102,62,0,
data byte = 0,96,96,124,102,102,124,0,
data byte = 0,0,60,96,96,96,60,0,
data byte = 0,6,6,62,102,102,62,0,
data byte = 0,0,60,102,126,96,60,0,
data byte = 0,14,24,62,24,24,24,0,
data byte = 0,0,62,102,102,62,6,124,
data byte = 0,96,96,124,102,102,102,0,
data byte = 0,24,0,56,24,24,60,0,
data byte = 0,6,0,6,6,6,6,60,
data byte = 0,96,96,108,120,108,102,0,
data byte = 0,56,24,24,24,24,60,0,
data byte = 0,0,102,127,127,107,99,0,
data byte = 0,0,124,102,102,102,102,0,
data byte = 0,0,60,102,102,102,60,0,
data byte = 0,0,124,102,102,124,96,96,
data byte = 0,0,62,102,102,62,6,6,
data byte = 0,0,124,102,96,96,96,0,
data byte = 0,0,62,96,60,6,124,0,
data byte = 0,24,126,24,24,24,14,0,
data byte = 0,0,102,102,102,102,62,0,
data byte = 0,0,102,102,102,60,24,0,
data byte = 0,0,99,107,127,62,54,0,
data byte = 0,0,102,60,24,60,102,0,
data byte = 0,0,102,102,102,62,12,120,
data byte = 0,0,126,12,24,48,126,0,
data byte = 0,24,60,126,126,24,60,0,
data byte = 24,24,24,24,24,24,24,24,
data byte = 0,126,120,124,110,102,6,0,
data byte = 8,24,56,120,56,24,8,0,
data byte = 16,24,28,30,28,24,16,0

' COPY PIECES CHARACTERS TO RAM
MOVE &PIECES, CHRAM, 1024

' MODIFY THE CHARACTER SET POINTER SO T
POKE 756,NMEMTOP





' BOARD SETUP `````````````````````````````````````````````
DATA CURSOR_X() = 82,94,106,118,130,142,154,166
DATA CURSOR_Y() = 89,81,73,65,57,49,41,33
DATA TILE_X() =  8,11,14,17,20,23,26,29
DATA TILE_Y() = 17,15,13,11,9,7,5,3

'DATA FILE_X() =  97,98,99,100,101,102,103,104
'DATA RANK_Y() =  49,50,51,52,53,54,55,56
FILE_X = 97
RANK_Y = 49

DIM BOARD_DATA(64) BYTE ' 8x8 board
MSET Adr( BOARD_DATA ), 64, 0 

POKE 559, 0

poke 87,1
'POS. 3,0 : ?#6, COLOR(128) "fujifish chess"
POS. 0,1 : ?#6,  "PLAYER: "
poke 87,0

@SETUP_BOARD

@DRAW_BOARD

POKE 559, 34


' PM GRAPHICS SETUP ```````````````````````````````````````
PMGRAPHICS 2   ' DOUBLE LINE MODE
p0addr = PMADR(0)  ' get player addresses
p1addr = PMADR(1)  
MSET p0addr, 128, 0 ' SET all bytes to 0
MSET p1addr, 128, 0 '
SETCOLOR -4, 3, 14
SETCOLOR -3, 4, 12
DATA PMCURSORDATA() BYTE= $FF,$81,$81,$81,$81, $FF
DATA PMSELECTDATA() BYTE= $C3,$81,$00,$00,$81, $C3
cursor_array_col = 4
cursor_array_row = 3
select_array_col = -1
select_array_row = -1
old_y = p0addr + CURSOR_Y(cursor_array_row)
old_sel_y = p1addr

@MOVEC


' NETWORK SETUP ```````````````````````````````````````````
'POS. 0,20 : ?#6,"enter host:"
POS. 0,21
INPUT "Enter Host:";GAMEHOST$

MODE$ = "S"
SIDE$ = "W"
LEVEL$= "5"
GAMEID$ = ""
POS. 0,21
INPUT "(S)tart or (J)oin Game?:";ANS$
if ANS$ = "S"
  POS. 0,21: INPUT "(O)ne or (T)wo Players?:";ANS$
  IF ANS$ = "T"
    MODE$ = "D"
  ELSE
    POS. 0,21: INPUT "Skill Level (1-10)?:";LEVEL$
  ENDIF
ELSE
  POS. 0,21: INPUT "Enter GameID:";GAMEID$
ENDIF

FJ_CONN = 2 ' unit 2
FJ_POST_MODE = 13 ' HTTP POST
FJ_GET_MODE = 12  ' HTTP GET https://fujinet.online/2025/02/23/developers-when-doing-http-get-use-mode-12/
FJ_TRANSL = 2 '  
FJ_BASE_URL$="N:HTTP://"
if LEN(GAMEHOST$) > 8
  FJ_BASE_URL$=+ GAMEHOST$
  FJ_BASE_URL$=+ ":55557/"
  ? "USING: ";GAMEHOST$
else
  POS.0,21
  FJ_BASE_URL$="N:HTTP://10.25.50.61:55557/"
  ? "USING default"
endif
' FJ_URL2$="N:HTTP://10.25.50.61:55557/newgame"$9B
' FJ_URL2$="N:HTTP://10.25.50.61:55557/move"$9B
' FJ_URL2$="N:HTTP://10.25.50.61:55557/board"$9B

DIM FJ_IN_BUFF(128) BYTE  
DIM FJ_OUT_BUFF(128) BYTE  
DIM GAME_ID(9) BYTE
DIM PLAYER_ID(9) BYTE

'quietus
POKE 65,0

if LEN(GAMEID$) < 8
  ' start a game
  FJ_OUT_BUFF(0) = ASC(MODE$)
  FJ_OUT_BUFF(1) = 10
  FJ_OUT_BUFF(2) = ASC(SIDE$)
  FJ_OUT_BUFF(3) = 10
  FJ_OUT_BUFF(4) = ASC(LEVEL$)
  FJ_OUT_BUFF(5) = 10

  @doPost &"newgame", Adr(FJ_OUT_BUFF), 6

  MOVE Adr(FJ_IN_BUFF), Adr(FJ_OUT_BUFF), 17 
  IF FJ_IN_BUFF(0) > 0 
    FJ_OUT_BUFF(8) = 10  ' \n
    FJ_OUT_BUFF(17) = 10  ' \n
    GAME_ID(0) = 8
    MOVE Adr(FJ_IN_BUFF), Adr(GAME_ID)+1, 8
    PLAYER_ID(0) = 8
    MOVE Adr(FJ_IN_BUFF)+9, Adr(PLAYER_ID)+1, 8

    poke 87,1
    POS. 4,0 : ?#6, $( ADR( GAME_ID ) )
    poke 87,0

  ELSE
    POS. 0,22 : PRINT "Unable to connect"
  ENDIF
  who_am_i = 1
  
ELSE
  ' JOIN a game
  MOVE Adr(GAMEID$)+1, Adr(FJ_OUT_BUFF), 8
  FJ_OUT_BUFF(8) = 10
  @doPost &"joingame", Adr(FJ_OUT_BUFF), 9

  IF FJ_IN_BUFF(0) > 0 
    MOVE Adr(FJ_IN_BUFF), Adr(FJ_OUT_BUFF)+9, 8 
    FJ_OUT_BUFF(17) = 10  ' \n
    MOVE Adr(GAMEID$)+1, Adr(GAME_ID)+1, 8
    GAME_ID(0) = 8
    PLAYER_ID(0) = 8
    MOVE Adr(FJ_IN_BUFF), Adr(PLAYER_ID)+1, 8
  ELSE
    POS. 0,22 : PRINT "Unable to connect"
  ENDIF
  who_am_i = 2
   
ENDIF

current_player = 0


' MAIN LOOP ```````````````````````````````````````````````
DO 
  ' can we move?
  if current_player = who_am_i 
    ' We're the current player, so read user input

    ' check button press
    btn_pressed = STRIG(0)
    if btn_pressed = 0
      if select_array_col < 0

        ' no pieces are selected at the moment. Make sure we're on our piece
        current_piece = BOARD_DATA( cursor_array_col + cursor_array_row * 8 )
        if ( who_am_i = 1  and current_piece > 6  and current_piece < 13 ) or ( who_am_i = 2 and current_piece > 0  and current_piece < 7 )
          ' our piece, select it
          select_array_col = cursor_array_col
          select_array_row = cursor_array_row
          sound 0,104,10,4
          pause 1
          sound 0,0,0,0
        else
          sound 0,32,2,4
          pause 1
          sound 0,0,0,0
        endif
        pause 9

      else  ' if select_array_col < 0

        ' Had a piece selected,  submit possible move. Server will let us know if it's valid
        FJ_OUT_BUFF(18) = FILE_X + select_array_col 
        FJ_OUT_BUFF(19) = RANK_Y + select_array_row 
        FJ_OUT_BUFF(20) = FILE_X + cursor_array_col
        FJ_OUT_BUFF(21) = RANK_Y + cursor_array_row
        FJ_OUT_BUFF(22) = 10
        @doPost &"move", ADR( FJ_OUT_BUFF ), 23


        IF FJ_IN_BUFF(0) > 0 
          ' is the value a legal  move?
          if FJ_IN_BUFF(0) = 108 and FJ_IN_BUFF(1) = 101 and FJ_IN_BUFF(2) = 103 and FJ_IN_BUFF(3) = 97 and FJ_IN_BUFF(4) = 108
            ' 'legal' move returned. Do the local move.
            @uci_move select_array_col, select_array_row, cursor_array_col, cursor_array_row
 
            if MODE$ = "S"  
              current_player = 2
              POS. 0,1 : ?#6,  "WAITING    "
            elif MODE$ = "D"  ' two player returns lichess move, but I should stop doing this.
              poke 87,1
              if current_player = 1
                current_player = 2
                POS. 0,1 : ?#6,  "PLAYER: TWO"
              else
                current_player = 1
                POS. 0,1 : ?#6,  "PLAYER: ONE"
              endif
              poke 87,0
            endif
          else
            POS. 0,21 : PRINT "Invalid move            "
          endif
        ELSE

          POS. 0,21 : PRINT "Lost connection            "

        ENDIF ' FU_IN_BUFF(0) > 0'

        ' clear selected piece.
        select_array_col = -1
        select_array_row = -1
        MSET old_sel_y, 6, 0  

        pause 10

      endif  ' if select_array_col < 0
    endif ' if btn_pressed = 0

    ' check for movement
    I = STICK(0)
    if I = 15
      sound 0,0,0,0 ' no motion, cancel any sound from previous movement
    else
      ' compute the next position
      cursor_array_col = cursor_array_col + ((I=7) -(I=11))
      if cursor_array_col > 7
        cursor_array_col = 0
      endif
      if cursor_array_col < 0
        cursor_array_col = 7
      endif
 
      cursor_array_row = cursor_array_row + ((I=14) -(I=13))
      if cursor_array_row > 7
        cursor_array_row = 0
      endif
      if cursor_array_row < 0
        cursor_array_row = 7
      endif

      ' POS. 0,21 : PRINT "X: ";cursor_array_col;" Y: ";cursor_array_row
      sound 0,180,2,4
      pause 1
      sound 0,0,0,0
      pause 9
      @MOVEC
    endif
  else  ' if current_player = who_am_i 

    ' we're not current player,  POLL status 
    query$ = "gid="
    query$ =+ $( ADR( GAME_ID ) )
    FJ_IN_BUFF(0) = 0
    @doGet &"status", &(query$)


    ' check return value starts with (T)urn 84 or (O)ver 79.  If Over check for win condition 3
    IF FJ_IN_BUFF(0) = 84 
      ' 'T'
      '              111111111122225
      '    0123456789012345678901234
      '    TURN w:LAST e7e6:MVNO 2
      if FJ_IN_BUFF(5) = 119 ' w
        current_player = 1
      elif FJ_IN_BUFF(5) = 98 ' b
        current_player = 2
      else   
        current_player = 0   ' all other conditions 
      endif 

      ' parse move 
      ' compute returns the move.
      @uci_move FJ_IN_BUFF(12)-FILE_X, FJ_IN_BUFF(13)-RANK_Y, FJ_IN_BUFF(14)-FILE_X, FJ_IN_BUFF(15) - RANK_Y

      poke 87,1
      if current_player = 1
        POS. 0,1 : ?#6,  "PLAYER: ONE"
      elif current_player = 2
        POS. 0,1 : ?#6,  "PLAYER: TWO"
      else
        POS. 0,1 : ?#6,  "PLAYER:    "
      endif
      poke 87,0
  
    elif FJ_IN_BUFF(0) = 79
      ' 'O'
      '    0123456789
      '    OVER 0-1 1:TURN w:LAST d8h4:MVNO 4
      '
      '    1-0, 0-1,  1/2-1/2   (so we can check position 7 for 0,1,2 (who won)
      '
      '    position 9 - 1 = checkmate, 2 = stalemate, 3 - insufficient material, 4- seventyfive moves, 5 - repetition, 6 fifty moves, 6 reps, 8 variant wih, 9 variant loss 10 variant draw
      if FJ_IN_BUFF(7) = 49  '0
        ' white wins
      elif FJ_IN_BUFF(7) = 49  '1
        ' black wins
      elif FJ_IN_BUFF(7) = 49  '2
        ' draw
      endif
    ENDIF 


 
    if current_player <> who_am_i
      ' hide cursors if it's not our turn.
      MSET old_y, 6, 0  ' CLEAR old 
      MSET old_sel_y, 6, 0  
      pause 300 ' wait a bit before we check again.
    else
      ' Hello Me, it's me again.  show the cursor
      @MOVEC
    endif
  
  endif ' if current_player = who_am_i 
LOOP



' PROCS ```````````````````````````````````````````````````
PROC MOVEC
  PAUSE   'WAIT FOR BLANK
  MSET old_y, 6, 0  ' CLEAR old 
  PMHPOS 0, CURSOR_X(cursor_array_col)
  old_y = p0addr + CURSOR_Y(cursor_array_row)
  MOVE ADR(PMCURSORDATA), old_y, 6
  if select_array_col >= 0 
     PMHPOS 1, CURSOR_X(select_array_col)
     old_sel_y = p1addr + CURSOR_Y( select_array_row )
     MOVE ADR(PMSELECTDATA), old_sel_y, 6
  else  
     MSET old_sel_y, 6, 0  
  endif
ENDPROC


PROC DRAW_BOARD
  poke 87,0
  index = 0
  FOR row = 0 to 7 
    FOR col = 0 to 7
      if BOARD_DATA(index) = 0 
        if  row & 1 EXOR col & 1
          pos. TILE_X(col),   TILE_Y(row) : ? #6, chr$($21)
          pos. TILE_X(col)+1, TILE_Y(row) : ? #6, chr$($21)
          pos. TILE_X(col)+2, TILE_Y(row) : ? #6, chr$($21)
          pos. TILE_X(col),   TILE_Y(row)+1 : ? #6, chr$($21)
          pos. TILE_X(col)+1, TILE_Y(row)+1 : ? #6, chr$($21)
          pos. TILE_X(col)+2, TILE_Y(row)+1 : ? #6, chr$($21)
        else
          pos. TILE_X(col),   TILE_Y(row) : ? #6, chr$($22)
          pos. TILE_X(col)+1, TILE_Y(row) : ? #6, chr$($22)
          pos. TILE_X(col)+2, TILE_Y(row) : ? #6, chr$($22)
          pos. TILE_X(col),   TILE_Y(row)+1 : ? #6, chr$($22)
          pos. TILE_X(col)+1, TILE_Y(row)+1 : ? #6, chr$($22)
          pos. TILE_X(col)+2, TILE_Y(row)+1 : ? #6, chr$($22)
        endif
      else 
        @DRAW_PIECE col, row, BOARD_DATA(index)
      endif
  

      index = index + 1
    NEXT col


  NEXT row
ENDPROC


PROC SETUP_BOARD 
  MSET Adr(BOARD_DATA), 64, 0 
  ' 
  ' 1 - black pawn 
  ' 2 - black knight 
  ' 3 - black bishop 
  ' 4 - black rook 
  ' 5 - black queen
  ' 6 - black king
  '
  ' 7 - white pawn 
  ' 8 - white knight 
  ' 9 - white bishop 
  ' 10- white rook 
  ' 11- white queen
  ' 12- white king
  '

  BOARD_DATA(0) = 10
  BOARD_DATA(1) = 8   
  BOARD_DATA(2) = 9
  BOARD_DATA(3) = 11
  BOARD_DATA(4) = 12
  BOARD_DATA(5) = 9
  BOARD_DATA(6) = 8
  BOARD_DATA(7) = 10

  BOARD_DATA(8) =   7 
  BOARD_DATA(9) =   7  
  BOARD_DATA(10) =  7 
  BOARD_DATA(11) =  7 
  BOARD_DATA(12) =  7 
  BOARD_DATA(13) =  7 
  BOARD_DATA(14) =  7 
  BOARD_DATA(15) =  7 

  BOARD_DATA(48) = 1
  BOARD_DATA(49) = 1
  BOARD_DATA(50) = 1
  BOARD_DATA(51) = 1
  BOARD_DATA(52) = 1
  BOARD_DATA(53) = 1
  BOARD_DATA(54) = 1
  BOARD_DATA(55) = 1

  BOARD_DATA(56) = 4 
  BOARD_DATA(57) = 2 
  BOARD_DATA(58) = 3 
  BOARD_DATA(59) = 5 
  BOARD_DATA(60) = 6 
  BOARD_DATA(61) = 3 
  BOARD_DATA(62) = 2 
  BOARD_DATA(63) = 4 

ENDPROC

PROC uci_move bx1 by1 bx2 by2
  ' get type at start
  x = TILE_X( bx1 )
  y = TILE_Y( by1 )
  current_piece = BOARD_DATA( bx1 + by1 * 8 )
  BOARD_DATA( bx1 + by1 * 8 ) = 0
  BOARD_DATA( bx2 + by2 * 8 ) = current_piece 
  @DRAW_TILE  bx1, by1
  @DRAW_PIECE bx2, by2, current_piece
 
  ' castles to check  
  ' white
  ' e1g1 -  4,0,6,0
  ' e1c1 -  4,0,2,0 
  ' black
  ' e8g8 -  4,7,6,7
  ' e8c8 -  4,7,2,7 
  if bx1 = 4 and by1 = 0 and bx2 = 6 and by2 = 0
    ' move rook from right
    BOARD_DATA( 7 ) = 0
    BOARD_DATA( 5 ) = 10
    x = TILE_X( 7 )
    y = TILE_Y( 0 )
    @DRAW_TILE 7, 0
    @DRAW_PIECE 5, 0, 10

  elif bx1 = 4 and by1 = 0 and bx2 = 2 and by2 = 0
    ' move rook from left
    BOARD_DATA( 0 ) = 0
    BOARD_DATA( 3 ) = 10
    x = TILE_X( 0 )
    y = TILE_Y( 0 )
    @DRAW_TILE 0, 0
    @DRAW_PIECE 3, 0, 10
  elif bx1 = 4 and by1 = 7 and bx2 = 6 and by2 = 7
    ' move rook from right
    BOARD_DATA( 63 ) = 0
    BOARD_DATA( 61 ) = 4
    x = TILE_X( 7 )
    y = TILE_Y( 7 )
    @DRAW_TILE 7, 7
    @DRAW_PIECE 5, 7, 4

  elif bx1 = 4 and by1 = 7 and bx2 = 2 and by2 = 7
    ' move rook from left
    BOARD_DATA( 56 ) = 0
    BOARD_DATA( 59 ) = 4
    x = TILE_X( 0 )
    y = TILE_Y( 7 )
    @DRAW_TILE 0, 7
    @DRAW_PIECE 3, 7, 4

  endif

  'TODO PROMOTION MOVE

ENDPROC

PROC DRAW_TILE tx ty
  if tx & 1 EXOR ty & 1
    pos. x,   y : ? #6, chr$($21)
    pos. x+1, y : ? #6, chr$($21)
    pos. x+2, y : ? #6, chr$($21)
    pos. x+2, y : ? #6, chr$($21) ' double up because of weird invers behavior
    pos. x,   y+1 : ? #6, chr$($21)
    pos. x+1, y+1 : ? #6, chr$($21)
    pos. x+2, y+1 : ? #6, chr$($21)
    pos. x+2, y+1 : ? #6, chr$($21) ' double up because of weird invers behavior
  else
    pos. x,   y : ? #6, chr$($22)
    pos. x+1, y : ? #6, chr$($22)
    pos. x+2, y : ? #6, chr$($22)
    pos. x+2, y : ? #6, chr$($22) ' double up because of weird invers behavior
    pos. x,   y+1 : ? #6, chr$($22)
    pos. x+1, y+1 : ? #6, chr$($22)
    pos. x+2, y+1 : ? #6, chr$($22)
    pos. x+2, y+1 : ? #6, chr$($22) ' double up because of weird invers behavior
  endif

ENDPROC

PROC DRAW_PIECE board_x board_y type
  poke 87,0
  x = TILE_X( board_x )
  y = TILE_Y( board_y )
  
  'light_square = 0
  'if index & 1
  '  light_square = 1
  'endif
  color_adjust = 0
  if type > 6 then color_adjust = $80

  draw_light = board_x & 1 EXOR board_y & 1

  if type = 1 OR type = 7 ' black pawn
    if draw_light 
       t1 = ($21+color_adjust)
       t2 = ($24+color_adjust) ' pawn top light square
       t3 = ($21+color_adjust)
       t4 = ($21+color_adjust)
       t5 = ($23+color_adjust) ' pawn bottom light square
       t6 = ($21+color_adjust)
    else
       t1 = ($22+color_adjust)
       t2 = ($26+color_adjust) ' pawn top dark square
       t3 = ($22+color_adjust)
       t4 = ($22+color_adjust)
       t5 = ($25+color_adjust) ' pawn bottom dark square
       t6 = ($22+color_adjust)
    endif
  elif type = 2 OR type = 8   ' knight
    if draw_light 
      t1 = ($16+color_adjust)
      t2 = ($17+color_adjust) 
      t3 = ($18+color_adjust)
      t4 = ($14+color_adjust)
      t5 = ($09+color_adjust)
      t6 = ($15+color_adjust)
    else   
      t1 = ($1E+color_adjust)
      t2 = ($1C+color_adjust) 
      t3 = ($1D+color_adjust)
      t4 = ($19+color_adjust)
      t5 = ($0F+color_adjust) 
      t6 = ($1A+color_adjust)
    endif
  elif type = 3 OR type = 9   ' bish
    if draw_light 
      t1 = ($0B+color_adjust)
      t2 = ($0C+color_adjust) ' bishop top light square
      t3 = ($0D+color_adjust)
      t4 = ($08+color_adjust)
      t5 = ($09+color_adjust) ' bishop (bishop|knight) bottom light square
      t6 = ($0A+color_adjust)
    else  
      t1 = ($11+color_adjust)
      t2 = ($12+color_adjust) ' bishop top dark square
      t3 = ($13+color_adjust)
      t4 = ($0E+color_adjust)
      t5 = ($0F+color_adjust) ' bishop (bishop|knight) bottom dark square
      t6 = ($10+color_adjust)
    endif
  elif type = 4 OR type = 10   ' rook
    if draw_light 
      t1 = ($21+color_adjust)
      t2 = ($04+color_adjust) ' rook top light square
      t3 = ($05+color_adjust)
      t4 = ($27+color_adjust)
      t5 = ($28+color_adjust) ' rook (queen|rook) bottom light square
      t6 = ($29+color_adjust)
    else                    
      t1 = ($22+color_adjust)
      t2 = ($06+color_adjust) ' rook top dark square
      t3 = ($07+color_adjust)
      t4 = ($2D+color_adjust)
      t5 = ($2E+color_adjust) ' rook (queen|rook) bottom dark square
      t6 = ($2F+color_adjust)
    endif
  elif type = 5 OR type = 11   ' queen
    if draw_light 
      t1 = ($2A+color_adjust)
      t2 = ($2B+color_adjust) ' queen top light square
      t3 = ($2C+color_adjust)
      t4 = ($27+color_adjust)
      t5 = ($28+color_adjust) ' queen bottom light square
      t6 = ($29+color_adjust)
    else                    
      t1 = ($5B+color_adjust)
      t2 = ($5C+color_adjust) ' queen top dark square
      t3 = ($5D+color_adjust)
      t4 = ($2D+color_adjust)
      t5 = ($2E+color_adjust) ' queen bottom dark square
      t6 = ($2F+color_adjust)
    endif
  elif type = 6 OR type = 12   ' king
    if draw_light 
      t1 = ($5E+color_adjust)
      t2 = ($5F+color_adjust) ' king top light square
      t3 = ($00+color_adjust)
      t4 = ($27+color_adjust)
      t5 = ($28+color_adjust) ' king (queen|rook) bottom light square
      t6 = ($29+color_adjust)
    else                    
      t1 = ($01+color_adjust)
      t2 = ($02+color_adjust) ' king top dark square
      t3 = ($03+color_adjust)
      t4 = ($2D+color_adjust)
      t5 = ($2E+color_adjust) ' king (queen|rook) bottom dark square
      t6 = ($2F+color_adjust)
    endif
  endif
  @place_tiles
ENDPROC

PROC place_tiles 
     pos.  x  ,y   : ? #6, chr$( t1 )
     pos.  x+1,y   : ? #6, chr$( t2 ) ' pawn top light square
     pos.  x+2,y   : ? #6, chr$( t3 )
     pos.  x+2,y   : ? #6, chr$( t3 ) ' double up because of weird invers behavior
     pos.  x  ,y+1 : ? #6, chr$( t4 )
     pos.  x+1,y+1 : ? #6, chr$( t5 ) ' pawn bottom light square
     pos.  x+2,y+1 : ? #6, chr$( t6 )
     pos.  x+2,y+1 : ? #6, chr$( t6 ) ' double up because of weird invers behavior
ENDPROC


PROC doPost endpoint buffer buffer_len
  url$ = FJ_BASE_URL$
  url$ =+ $(endpoint)
  url$ =+ ""$9B

  POS. 0,21 : PRINT "Try: ";url$
  NOPEN FJ_CONN, FJ_POST_MODE, FJ_TRANSL, url$
  IF SERR() <> 1
    ' doesn't actually seem ot detect a problem connecting.
    'POS. 0,21 : PRINT "Unable to open connection"
    Exit
  ENDIF
  'POS. 0,21 : PRINT "Connected           "

  ' create post data
  SIO $71, FJ_CONN, $4D, $00, 0, $1F, 0, FJ_POST_MODE, 4
  NPUT FJ_CONN, buffer, buffer_len
  ' get response body
  SIO $71, FJ_CONN, $4D, $00, 0, $1F, 0, FJ_POST_MODE, 0
  TRIES = 0
  DO
    NSTATUS FJ_CONN  ' NSTATUS followd by DPEEK($02EA)
    BW = DPEEK($02EA)
    IF BW > 0 THEN EXIT
    pause 1
    TRIES = TRIES + 1
    IF TRIES > 100 THEN EXIT
  LOOP 
  IF BW = 0
    MSET ADR( FJ_IN_BUFF ), 64, 0
    EXIT
  ENDIF
  ' loop the read
  WHILE BW
    IF BW > 8192 OR BW < 000
      LN = 8192
    ELSE
      LN = BW
    ENDIF
    
    NGET FJ_CONN,&FJ_IN_BUFF, LN
    BW = BW - LN
  WEND
  NCLOSE FJ_CONN
ENDPROC


PROC doGet endpoint param
  url$ = FJ_BASE_URL$
  url$ =+ $(endpoint)
  url$ =+ "?"
  url$ =+ $(param)
  url$ =+ ""$9B
  
  NOPEN FJ_CONN, FJ_GET_MODE, FJ_TRANSL, URL$
  NSTATUS FJ_CONN
  IF SERR() <> 1
    PRINT "Unable to get connection"
  ENDIF
  ' get response body
  SIO $71, FJ_CONN, $4D, $00, 0, $1F, 0, FJ_GET_MODE, 0
  DO
    NSTATUS FJ_CONN  ' NSTATUS followd by DPEEK($02EA)
    BW = DPEEK($02EA)
    IF BW > 0 THEN EXIT
  LOOP 
  ' loop the read
  WHILE BW
    IF BW > 8192 OR BW < 000
      LN = 8192
    ELSE
      LN = BW
    ENDIF
    
    NGET FJ_CONN,&FJ_IN_BUFF, LN
    BW = BW - LN
  WEND
  NCLOSE FJ_CONN
ENDPROC


PROC INTCLR
  POKE $D302, PEEK($D302) & 127  ' $D302 - PIA control
ENDPROC


