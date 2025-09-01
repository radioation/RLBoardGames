' 
CONN=2   ' unit 2
MODE=12  ' mode, 12 is read/write
TRANS=0  ' translation mode, 0 no translation
URL$="N:TCP://10.25.50.61:65432"  ' URL 

' receive buffer
DIM BUFF(8192) BYTE


' PROCEDURES '''
PROC TITLE
  ? "** ECHO TEST **"
  ?
ENDPROC


PROC INTCLR
  POKE $D302, PEEK($D302) & 127  ' $D302 - PIA control
ENDPROC


' MAIN ''''
@TITLE

' open the connection
NOPEN CONN,MODE,TRANS,URL$

NSTATUS CONN

'' clear interrupt? do I need this?
@INTCLR

DO
  ' read  from keyboard
  INPUT "Enter Message: ", MSG$
  
  ' write to server (strings seem to sned a number
  NPUT CONN,&MSG$,LEN(MSG$)+1
  
  @INTCLR
  
  ' get number of Bytes Waiting in the receive buffer
  DO
    NSTATUS CONN  ' NSTATUS followd by DPEEK($02EA)
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
    
    NGET CONN,&BUFF, LN
    BPUT #0, &BUFF, LN
    BW = BW - LN
  WEND
  print "" 
LOOP

NCLOSE CONN





