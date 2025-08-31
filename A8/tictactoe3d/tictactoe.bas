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
SE. 0,12,12   ' O
SE. 1,4,12     ' X
SE. 2,9,6     ' LINES
SE. 3,9,6
SE. 4,9,0
ENDIF

'DATA FOR TIC TAC TOE TILES.
DATA CUSTOM() BYTE = 0,3,3,12,12,48,48,192, ' #  is /
DATA BYTE = 255,0,0,0,0,0,0,0,              ' $  is -
DATA BYTE = 255,3,3,12,12,48,48,192,        ' %  is 7
DATA BYTE = 255,0,21,65,65,65,84,0,         ' &  is O 
DATA BYTE = 255,0,34,34,40,136,136,0,       ' '  is X
DATA BYTE = 192,0,0,0,0,0,0,0               ' (  is `

' COPY THE ROM CHARACTER SET TO RAM
MOVE CHROM, CHRAM, 1024

' COPY CUSTOM CHARACTERS TO RAM
START=NMEMTOP*256+(8*3)
MOVE &CUSTOM, START, 48 '6 CUSTOM = 6*8

' MODIFY THE CHARACTER SET POINTER SO T
POKE 756,NMEMTOP
' PM GRAPHICS
PMGRAPHICS 1        ' SINGLE LINE MODE
P0MEM = PMADR(0)    ' PLAYER 0 ADRESS

MSET P0MEM, 256, 0  ' CLEAR ALL 256 BYT
SETCOLOR -4,1,15    ' SET P0 COLOR
DATA PMDATA() BYTE = 31,62,62,124,120

' CURSOR SETTINGS
x = 0
y = 0
layer = 0
OLDCY = 0
DATA CURSOR_X() BYTE = 114,122,130,138,110,118,126,134,106,114,122,130,102,110,118,126
DATA CURSOR_Y() BYTE = 34,42,50,58,74,82,90,98,114,122,130,138,154,162,170,178

DATA TILE_X() BYTE = 17,19,21,23,16,18,20,22,15,17,19,21,14,16,18,20
DATA TILE_Y() BYTE = 0,0,0,0,5,5,5,5,10,10,10,10,15,15,15,15

current_color = 1
current_tick = 0
cursor_input_delay = 80  ' delay

PRINT "KUMUSTA"

@DRAWBOARD

DO
  ' color cycle the cursor
  current_color = current_color + 1
  IF current_color > 15
    current_color = 1
  ENDIF
  SETCOLOR -4,current_color,15

  ' time delay 
  current_tick = current_tick + 1
  IF current_tick > cursor_input_delay 
    btn_pressed = STRIG(0)
    if btn_pressed = 0
      print "FIRE"
      POS. TILE_X( x ), TILE_Y( y ): ?#6,"&"  
    endif
    I = STICK(0)
    'CX = CX + 8 * ((I=7)-(I=11))
    x = x + ((I=7)-(I=11))
    IF x > 3 
      x = 0
      layer = layer + 1
    ENDIF 
    IF X < 0 
      x = 3
      layer = layer - 1
    ENDIF 
    'CY = CY + 8 * ((I=13)-(I=14))
    y = y + ((I=13)-(I=14))
    IF y > 3 
      y = 0
      layer = layer + 1
    ENDIF 

    IF y < 0 
      y = 3
      layer = layer - 1
    ENDIF 

    IF layer < 0 THEN layer = 3
    IF layer > 3 THEN layer = 0
    CY = CURSOR_Y( y + layer * 4 )
    CX = CURSOR_X( x + y * 4 )
    
    @MOVEC
    current_tick = 0
  ENDIF
LOOP


PROC DRAWBOARD
  FOR R=0 TO 15 STEP 5
    POS. 16,R+0:? #6,"#$%$%$%$%("
    POS. 15,R+1:? #6,"#$%$%$%$%("
    POS. 14,R+2:? #6,"#$%$%$%$%("
    POS. 13,R+3:? #6,"#$%$%$%$%("
    POS. 13,R+4:? #6,"$$$$$$$$("
  NEXT R
ENDPROC

PROC MOVEC
  MSET OLDCY, 5, 0  ' CLEAR 5 BYTES OF SPRITE
  PAUSE   'WAIT FOR BLANK
  PMHPOS 0, CX
  OLDCY = P0MEM + CY
  MOVE ADR(PMDATA), OLDCY, 5
ENDPROC

