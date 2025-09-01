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
POKE 623,4          ' put playfield ahead of sprites

' CURSOR SETTINGS
x = 0
y = 0
layer = 0
OLDCY = 0
DATA CURSOR_X() BYTE = 114,122,130,138,110,118,126,134,106,114,122,130,102,110,118,126
DATA CURSOR_Y() BYTE = 34,42,50,58,74,82,90,98,114,122,130,138,154,162,170,178

DATA TILE_X() BYTE = 17,19,21,23,16,18,20,22,15,17,19,21,14,16,18,20
DATA TILE_Y() BYTE = 0,1,2,3,5,6,7,8,10,11,12,13,15,16,17,18

current_cursor_color = 1
current_tick = 0
cursor_input_delay = 80  ' delay


' allocate board to track moves
DIM BOARD_DATA(64) BYTE
MSET Adr(BOARD_DATA), 64, 0


' NETWORK SETTINGS
FJ_CONN = 2 ' unit 2
FJ_MODE = 12 ' 12 - read/write
FJ_TRANSL = 0  ' no translation 
FJ_URL$="N:TCP://10.25.50.67:5364"
DIM FJ_BUFF(64) BYTE


' start main processing '''''''''''''''''''''''''''''''''''''''
POKE 65,0 ' quiet

@DRAWBOARD

@GET_ADDR

@JOIN_GAME

current_player = 0

who_am_i = 1

DO
  ' color cycle the cursor
  current_cursor_color = current_cursor_color + 1
  IF current_cursor_color > 15
    current_cursor_color = 1
  ENDIF
  SETCOLOR -4,current_cursor_color,15

  IF who_am_i = current_player
    ' time delay for input
    current_tick = current_tick + 1
    IF current_tick > cursor_input_delay 
      btn_pressed = STRIG(0)
      if btn_pressed = 0
        current_piece = BOARD_DATA( x + y * 4 + layer * 16 )
     
        IF current_piece = 0  
          sound 0,124,2,10
          POS. TILE_X( x + y * 4 ), TILE_Y( y + layer * 4 )
          ?#6,"&"  
          BOARD_DATA( x + y * 4 + layer * 16 ) = 2
          current_player = 0
          FJ_BUFF(0) = 129
          FJ_BUFF(1) = 3
          FJ_BUFF(2) = x
          FJ_BUFF(3) = y
          FJ_BUFF(4) = layer
          NPUT FJ_CONN, &FJ_BUFF, 5
        else
          sound 0,32,2,8
          pause 10
        endif
      else 
        sound 0,0,0,0
      endif

      I = STICK(0)
      if I = 15
        sound 0,0,0,0
      else
        sound 0,84,10,4
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
        ' send data   
        
        IF layer < 0 THEN layer = 3
        IF layer > 3 THEN layer = 0
        CY = CURSOR_Y( y + layer * 4 )
        CX = CURSOR_X( x + y * 4 )
        @MOVEC
        
        FJ_BUFF(0) = 128
        FJ_BUFF(1) = 3
        FJ_BUFF(2) = x
        FJ_BUFF(3) = y
        FJ_BUFF(4) = layer
        NPUT FJ_CONN, &FJ_BUFF, 5
      endif
      current_tick = 0
    ENDIF
  ELSE ' I'm not the current player
    ' listen for data
    PRINT "LISTENING..."
    DO
      NSTATUS FJ_CONN ' NSTATNUS followed by DPEEK($02EA) to get buffer
      BW = DPEEK($02EA)
      
      IF BW > 4 THEN EXIT
    LOOP
    PRINT "GOT DATA...";BW
    
    ' loop the read
    WHILE BW
      IF BW > 64 
        LN = 64
      ELSE
        LN = BW
      ENDIF
      NGET FJ_CONN, &FJ_BUFF, LN
      BW = BW - LN
    WEND
    PRINT "READ: ";FJ_BUFF(0)
 
    if FJ_BUFF(0) = 128  
      ' move the cursor around
      x = FJ_BUFF(2) 
      y = FJ_BUFF(3) 
      layer = FJ_BUFF(4) 
      CY = CURSOR_Y( y + layer * 4 )
      CX = CURSOR_X( x + y * 4 )
      
      @MOVEC
    ELIF FJ_BUFF(0) = 129 OR FJ_BUFF(0) = 130 
      ' update cursor position AND update board
      x = FJ_BUFF(2) 
      y = FJ_BUFF(3) 
      layer = FJ_BUFF(4) 
      CY = CURSOR_Y( y + layer * 4 )
      CX = CURSOR_X( x + y * 4 )
      
      @MOVEC
      POS. TILE_X( x + y * 4 ), TILE_Y( y + layer * 4 )
      if current_player = 0
        ?#6,"'"  
        current_player = 1
        BOARD_DATA( x + y * 4 + layer * 16 ) = 1
      endif    
    ENDIF 
  ENDIF
LOOP

' PROCS '''''''''''''''''''''''''''''''''''''''''''''''''''''''''

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


PROC INTCLR
   POKE $D302, PEEK($D302) & 127 ' PIA contrl?
ENDPROC

PROC GET_ADDR
  INPUT "Enter IP Address: "; ADDRESS$
  IF LEN(ADDRESS$) > 1 
    FJ_URL$="N:TCP://"
    FJ_URL$ =+ ADDRESS$
    FJ_URL$ =+ ":5364"
  ELSE 
    PRINT "Using default: ";FJ_URL$
  ENDIF
ENDPROC

PROC JOIN_GAME
  PRINT "JOINING GAME... : "; FJ_URL$
  NOPEN FJ_CONN,FJ_MODE,FJ_TRANSL,FJ_URL$
  NSTATUS FJ_CONN
  PRINT "CONNCETED ..."
  @INTCLR ' is this needed?
ENDPROC
