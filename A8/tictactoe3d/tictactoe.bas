' SETUP MEMORY FOR CUSTOM CHARS
' MOVEPOKE 87, 0
NMEMTOP=PEEK(106)-4
POKE 106,NMEMTOP

'GRAPHICS 12      ' ANTIC MODE 4 with text window
GRAPHICS 12+16      ' ANTIC MODE 4

dl = peek( 560) + peek( 561)* 256 + 4 ' 4 to protect the 24 blanks at start of DL
poke dl-1, $46       ' poke the LMS antic mode + 64(dec) or 06 + 40 -> 46
mset dl+2, 21, $04    ' skip the address already in memory and copy iun antic mode 4 ($04)
poke dl+23, $02       ' lst two lines are gr.0 (antic mode 2)
poke dl+24, $02
poke dl+25, $41      ' 65 or $41 is JVB
poke dl+26, peek(560)
poke dl+27, peek(561)


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
DATA CURSOR_Y() BYTE = 50,58,66,74,90,98,106,114,130,138,146,154,170,178,186,194

DATA TILE_X() BYTE = 17,19,21,23,16,18,20,22,15,17,19,21,14,16,18,20
DATA TILE_Y() BYTE = 1,2,3,4, 6,7,8,9, 11,12,13,14, 16,17,18,19

current_cursor_color = 1
current_tick = 0
cursor_input_delay = 80  ' delay


' allocate board to track moves
DIM BOARD_DATA(64) BYTE    ' 4 * 4 * 4 analgous to `board` in MegaDrive
MSET Adr(BOARD_DATA), 64, 0


' NETWORK SETTINGS
FJ_CONN = 1 ' unit 2
FJ_MODE = 12 ' 12 - read/write
FJ_TRANS = 0  ' no translation 
FJ_URL$="N:TCP://10.25.50.67:55555"
DIM FJ_BUFF(2048) BYTE

' setup winline 
' Offsets for row cells in layer
DATA WINLINE_IN_LAYER () BYTE = 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15,
' Offsets for col cells in layer
DATA BYTE  = 0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
' offsets for diagonal cells in a layer
DATA  BYTE  = 0,5,10,15, 3,6,9,12


' Diagonal cells to cube corners
DATA WINLINE_MULTILAYER() BYTE = 0,21,42,63, 3,22,41,60, 12,25,38,51, 15,26,37,48, 
' Diagonal cells in col
DATA BYTE = 0,20,40,60, 1,21,41,61, 2,22,42,62, 3,23,43,63, 12,24,36,48, 13,25,37,49, 14,26,38,50, 15,27,39,51, 
' Diagonal cells in row
DATA BYTE = 0,17,34,51, 4,21,38,55, 8,25,42,59, 12,29,46,63, 3,18,33,48, 7,22,37,52, 11,26,41,56, 15,30,45,60




' start main processing '''''''''''''''''''''''''''''''''''''''
TPSCRN = PEEK(88) + PEEK(89) * 256  ' GETTING A NEGATIVE NUMBER
POKE 65,0 ' quiet
POKE 87,1
'POS. 3,0 : ?#6, "3D TIC TAC TOE"
'
SIO $70, FJ_CONN, $E8, $40, &FJ_BUFF, $0F, $88, 0, 0  
pause 30
POS. 0,0 : ?#6, "IP: ";FJ_BUFF(97);".";FJ_BUFF(98);".";FJ_BUFF(99);".";FJ_BUFF(100) 

POKE 87, 0  ' treat 4 ad 0

'IF TPSCRN > 0
'  TPSCRN = TPSCRN + 21 ' NEGATIVE NUBMER FOR TPSCRN SEEMS TO F THINGS UP.
'  poke 88, TPSCRN - ( int( TPSCRN/256) * 256) ' NGATIVE NUBMER FOR TPSCRN SEEMS TO F THINGS UP.
'  poke 89, int( TPSCRN/256)
'ELSE
'  DAMMIT = PEEK(88)
'  POKE 88, DAMMIT + 20
'ENDIF

' deal with mode 1 screen top
ORIG_SCTP = DPEEK(88)
SCTP = DPEEK(88) + 20
DPOKE 88, SCTP 


@DRAWBOARD


winner = 0
who_am_i = 0
@GET_ADDR

IF who_am_i = 1
  @HOST_GAME
ELSE
  @JOIN_GAME
ENDIF

' show the cursor.
CY = CURSOR_Y( y + layer * 4 )
CX = CURSOR_X( x + y * 4 )
@MOVEC


current_player = 1


banner_msg$ = "PLAYER "
banner_msg$ =+ str$(current_player)
@set_banner 


game_won = 0
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
      if game_won = 0
        btn_pressed = STRIG(0)
        if btn_pressed = 0
          current_piece = BOARD_DATA( x + y * 4 + layer * 16 )
          IF current_piece = 0  
            ' check for win
            @update_board
            @check_win current_player
            if winner = 0 
              ' not won, just send it and switch players
              sound 0,124,2,10
              '
              if current_player = 1
                current_player = 2
              else
                current_player = 1
              endif 
              banner_msg$ = "PLAYER "
              banner_msg$ =+ str$(current_player)
              @set_banner 

              
              FJ_BUFF(0) = 129
              FJ_BUFF(1) = 3
              FJ_BUFF(2) = x
              FJ_BUFF(3) = y
              FJ_BUFF(4) = layer
              NPUT FJ_CONN, &FJ_BUFF, 5
            else
              game_won = 1
              'POS.0,22 : PRINT  "Player ";current_player;" wins!"
              banner_msg$ = "PLAYER "
              banner_msg$ =+ str$(current_player)
              banner_msg$ =+ " WINS!"
              @set_banner 
              FJ_BUFF(0) = 130
              FJ_BUFF(1) = 3
              FJ_BUFF(2) = x
              FJ_BUFF(3) = y
              FJ_BUFF(4) = layer
              NPUT FJ_CONN, &FJ_BUFF, 5
            endif            
          else
            sound 0,32,2,8
            pause 10
          endif ' if current_piece = 0
        else 
          sound 0,0,0,0
        endif ' if btn_pressed = 0

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
          
          IF layer < 0 THEN layer = 3
          IF layer > 3 THEN layer = 0
          CY = CURSOR_Y( y + layer * 4 )
          CX = CURSOR_X( x + y * 4 )
          @MOVEC
          
          ' send move data   
          FJ_BUFF(0) = 128
          FJ_BUFF(1) = 3
          FJ_BUFF(2) = x
          FJ_BUFF(3) = y
          FJ_BUFF(4) = layer
          NPUT FJ_CONN, &FJ_BUFF, 5
        endif 'if I = 15  (stick behavior

      endif ' if game_won =0
      current_tick = 0
    ENDIF ' IF current_tick > cursor_input_delay 
  ELSE ' IF who_am_i = current_player
   ' I'm not the current player

    ' listen for data
    DO
      NSTATUS FJ_CONN ' NSTATNUS followed by DPEEK($02EA) to get buffer
      BW = DPEEK($02EA)
      IF BW > 4 THEN EXIT
    LOOP
    
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

      @update_board
      if FJ_BUFF(0)= 130
        game_won = 1
      endif
      if current_player = 1
        current_player = 2
      else
        current_player = 1
      endif 
      banner_msg$ = "PLAYER "
      banner_msg$ =+ str$(current_player)
      @set_banner 

    ENDIF ' if FJ_BUFF(0) = 128

  ENDIF ' IF who_am_i = current_player
LOOP

' PROCS '''''''''''''''''''''''''''''''''''''''''''''''''''''''''

PROC DRAWBOARD
  FOR R=1 TO 16 STEP 5
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
  POKE 87,0
  POS.0,22 : INPUT "HOST IP Addr: "; ADDRESS$
  IF LEN(ADDRESS$) > 1 
    FJ_URL$="N:TCP://"
    FJ_URL$ =+ ADDRESS$
    FJ_URL$ =+ ":55555"
    who_am_i = 2
  ELSE 
    FJ_URL$="N:TCP://:55555/"
    who_am_i = 1
  ENDIF
ENDPROC

PROC HOST_GAME
  POS.0,22: PRINT "WAIT.. ";FJ_URL$
  NOPEN FJ_CONN, FJ_MODE, FJ_TRANS, FJ_URL$
  NSTATUS FJ_CONN
  @INTCLR

  DO 
    NSTATUS FJ_CONN
    CONNECTED = PEEK($02EC)
    IF CONNECTED 
      POS.0,22: PRINT "Client connected.                    "
      ' accept conection by sending an 'A' command ot the appropratie N unit
      SIO $71, FJ_CONN, $41, $00, 0, $1f, 0, FJ_MODE, FJ_TRANS
      EXIT
    ENDIF 
  LOOP
  
  DO 
    ' look for 'C'
    DO 
      NSTATUS FJ_CONN
      BW = DPEEK($02EA)
      IF BW>0 THEN EXIT
    LOOP
    WHILE BW
      IF BW > 64 OR BW < 000
        LN = 64
      ELSE
        LN = BW
      ENDIF
 
      NGET FJ_CONN,&FJ_BUFF, LN
      BPUT #0, &FJ_BUFF, LN
      BW = BW - LN
    WEND
    if FJ_BUFF(0) = 67
      exit
    endif 
  LOOP
ENDPROC

PROC JOIN_GAME
  POS.0,22: PRINT "JOIN.. ";FJ_URL$
  NOPEN FJ_CONN,FJ_MODE,FJ_TRANS,FJ_URL$
  NSTATUS FJ_CONN
  @INTCLR ' is this needed?

  DO
    NSTATUS FJ_CONN
    CONNECTED = PEEK($02EC)
    IF CONNECTED
      POS.0,22: PRINT "Connected.                           "
      EXIT
    ENDIF
  LOOP
  pause 30
  FJ_BUFF(0) = 67
  NPUT FJ_CONN, &FJ_BUFF, 1
  'XIO ASC("A"),#1,12,2,"N:" ' accept
  'XIO 15,#1,12,2,"N:"       ' flush
  'SIO $71, FJ_CONN, 15, $00, 0, $1f, 0, FJ_MODE, FJ_TRANS
  @INTCLR
ENDPROC

PROC update_board
  POS. TILE_X( x + y * 4 ), TILE_Y( y + layer * 4 )
  BOARD_DATA( x + y * 4 + layer * 16 ) = current_player
  if current_player = 1
    ?#6,"'"  
  else
    ?#6,"&"  
  endif

ENDPROC

PROC check_win player

  winner = 0

  ' single layer checks
  for CW_Z =0 to 48 STEP 16 
    FOR I = 0 TO 36 STEP 4
      CW_COUNT = 0 
      FOR J=0 TO 3
        CW_INDEX = WINLINE_IN_LAYER( I + J ) + CW_Z 
        if BOARD_DATA(CW_INDEX)  = player
           CW_COUNT = CW_COUNT + 1
        endif
      NEXT J
      IF CW_COUNT = 4
        winner = player
        I = 28
        CW_Z = 48
        EXIT
      ENDIF
    NEXT I
  next CW_Z

  if winner > 0 
    exit
  endif

  ' straight downs.
  for I =0 to 15
    CW_COUNT = 0 
    for CW_Z =0 to 48 STEP 16 
      CW_INDEX =  I + CW_Z   
      if BOARD_DATA(CW_INDEX)  = player
         CW_COUNT = CW_COUNT + 1
      endif
    next CW_Z
    if CW_COUNT = 4
      winner = player
      exit
    ENDIF
  NEXT I

  ' diagnoals
  for I =0 to 76 STEP 4
    CW_COUNT = 0 
    for J =0 to 4 
      CW_INDEX = WINLINE_MULTILAYER( I + j )  
      if BOARD_DATA(CW_INDEX)  = player
         CW_COUNT = CW_COUNT + 1
      endif
    nextJ 
    if CW_COUNT = 4
      winner = player
      exit
    ENDIF
  NEXT I


ENDPROC


PROC set_banner 
  DPOKE 88, ORIG_SCTP 
  POS. 0,0 : ?#6, "                   "
  POS. 0,0 : ?#6, banner_msg$
  DPOKE 88, SCTP 
ENDPROC

