' SETUP MEMORY FOR CUSTOM CHARS
' MOVE MEMORY BY 4 PAGES
NMEMTOP=PEEK(106)-4
POKE 106,NMEMTOP


GRAPHICS 12         ' ANTIC MODE 4
CHROM=PEEK(756)*256 ' FIND LOCATION OF
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
DATA CURSOR_Y() = 21,29,37,45,53,61,69,77
DATA TILE_X() =  8,11,14,17,20,23,26,29
DATA TILE_Y() = 1,3,5,7,9,11,13,15

cursor_col = 4
cursor_row = 4
select_col = -1
select_row = -1


DIM BOARD_DATA(64) BYTE ' 8x8 board
MSET Adr( BOARD_DATA ), 64, 0 

POKE 559, 0
@SETUP_BOARD

@DRAW_BOARD

POKE 559, 34
' SETUP P/M Graphics
PMGRAPHICS 2   ' DOUBLE LINE MODE
p0addr = PMADR(0)  ' get player addresses
p1addr = PMADR(1)  
MSET p0addr, 128, 0 ' SET all bytes to 0
MSET p1addr, 128, 0 '
SETCOLOR -4, 0, 14
SETCOLOR -4, 3, 14
DATA PMCURSORDATA() BYTE= $FF,$FF,$FF,$FF,$FF, $FF
DATA PMSELECTDATA() BYTE= $FF,$81,$81,$81,$81, $FF
old_y = p0addr + CURSOR_Y(cursor_row)

@MOVEC

' MAIN LOOP ```````````````````````````````````````````````
DO 
  ' read user input
  btn_pressed = STRIG(0)
  if btn_pressed = 0
    
  endif
  I = STICK(0)
  if I = 15
    sound 0,0,0,0
  else
    cursor_col = cursor_col + ((I=7) -(I=11))
    if cursor_col > 7
      cursor_col = 0
    endif
    if cursor_col < 0
      cursor_col = 7
    endif

    cursor_row = cursor_row + ((I=13) -(I=14))
    if cursor_row > 7
      cursor_row = 0
    endif
    if cursor_row < 0
      cursor_row = 7
    endif
    pause 10
    @MOVEC
  endif
LOOP



' PROCS ```````````````````````````````````````````````````
PROC MOVEC
  PAUSE   'WAIT FOR BLANK
  MSET old_y, 6, 0  ' CLEAR 5 BYTES OF SPRITE
  PMHPOS 0, CURSOR_X(cursor_col)
  old_y = p0addr + CURSOR_Y(cursor_row)
  MOVE ADR(PMCURSORDATA), old_y, 6
ENDPROC


PROC DRAW_BOARD
  draw_light = 1
  index = 0
  FOR row = 0 to 7 
    FOR col = 0 to 7
      if BOARD_DATA(index) = 0 
        if draw_light = 1
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
  
      if draw_light = 1
        draw_light = 0
      else 
        draw_light = 1
      endif

      index = index + 1
    NEXT col

    if draw_light = 1
      draw_light = 0
    else
      draw_light = 1
    endif

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
  BOARD_DATA(0) = 4
  BOARD_DATA(1) = 2 
  BOARD_DATA(2) = 3
  BOARD_DATA(3) = 5
  BOARD_DATA(4) = 6
  BOARD_DATA(5) = 3
  BOARD_DATA(6) = 2
  BOARD_DATA(7) = 4

  BOARD_DATA(8) =   1
  BOARD_DATA(9) =   1 
  BOARD_DATA(10) =  1
  BOARD_DATA(11) =  1
  BOARD_DATA(12) =  1
  BOARD_DATA(13) =  1
  BOARD_DATA(14) =  1
  BOARD_DATA(15) =  1

  BOARD_DATA(48) = 7
  BOARD_DATA(49) = 7
  BOARD_DATA(50) = 7
  BOARD_DATA(51) = 7
  BOARD_DATA(52) = 7
  BOARD_DATA(53) = 7
  BOARD_DATA(54) = 7
  BOARD_DATA(55) = 7

  BOARD_DATA(56) = 10
  BOARD_DATA(57) = 8 
  BOARD_DATA(58) = 9
  BOARD_DATA(59) = 11
  BOARD_DATA(60) = 12
  BOARD_DATA(61) = 9
  BOARD_DATA(62) = 8
  BOARD_DATA(63) = 10

ENDPROC


PROC DRAW_PIECE board_x board_y type
  x = TILE_X( board_x )
  y = TILE_Y( board_y )
  
  'light_square = 0
  'if index & 1
  '  light_square = 1
  'endif
  color_adjust = 0
  if type > 6 then color_adjust = $80

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
     pos.  x  ,y+1 : ? #6, chr$( t4 )
     pos.  x+1,y+1 : ? #6, chr$( t5 ) ' pawn bottom light square
     pos.  x+2,y+1 : ? #6, chr$( t6 )
ENDPROC



