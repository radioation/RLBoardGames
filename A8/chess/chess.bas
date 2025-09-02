
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
data byte = 170,170,190,255,239,255,255,255,     ' $1C $9C : 86 : knight top 2 dark
data byte = 170,170,170,170,234,234,250,250,     ' $1D $9D : 86 : knight top 3 dark
data byte = 0,24,48,126,48,24,0,0,                
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

' COPY THE ROM CHARACTER SET TO RAM
'MOVE CHROM, CHRAM, 1024

' COPY PIECES CHARACTERS TO RAM
'START=NMEMTOP*256+(8*3)
'MOVE &PIECES, START, 144 '18 PIECES = 18*8
MOVE &PIECES, CHRAM, 1024

' MODIFY THE CHARACTER SET POINTER SO T
POKE 756,NMEMTOP


' pawn test
pos.  1,1 : ? #6, chr$($21)
pos.  2,1 : ? #6, chr$($24) ' pawn top light square
pos.  3,1 : ? #6, chr$($21)
pos.  1,2 : ? #6, chr$($21)
pos.  2,2 : ? #6, chr$($23) ' pawn bottom light square
pos.  3,2 : ? #6, chr$($21)

pos.  4,1 : ? #6, chr$($22)
pos.  5,1 : ? #6, chr$($26) ' pawn top dark square
pos.  6,1 : ? #6, chr$($22)
pos.  4,2 : ? #6, chr$($22)
pos.  5,2 : ? #6, chr$($25) ' pawn bottom dark square
pos.  6,2 : ? #6, chr$($22)

pos.  7,1 : ? #6, chr$($A1)
pos.  8,1 : ? #6, chr$($A4) ' pawn top light square
pos.  9,1 : ? #6, chr$($A1)
pos.  7,2 : ? #6, chr$($A1)
pos.  8,2 : ? #6, chr$($A3) ' pawn bottom light square
pos.  9,2 : ? #6, chr$($A1)

pos. 10,1 : ? #6, chr$($A2)
pos. 11,1 : ? #6, chr$($A6) ' pawn top dark square
pos. 12,1 : ? #6, chr$($A2)
pos. 10,2 : ? #6, chr$($A2)
pos. 11,2 : ? #6, chr$($A5) ' pawn bottom dark square
pos. 12,2 : ? #6, chr$($A2)

' Queen test
pos.  1,3 : ? #6, chr$($2A)
pos.  2,3 : ? #6, chr$($2B) ' queen top light square
pos.  3,3 : ? #6, chr$($2C)
pos.  1,4 : ? #6, chr$($27)
pos.  2,4 : ? #6, chr$($28) ' queen bottom light square
pos.  3,4 : ? #6, chr$($29)

pos.  4,3 : ? #6, chr$($5B)
pos.  5,3 : ? #6, chr$($5C) ' queen top dark square
pos.  6,3 : ? #6, chr$($5D)
pos.  4,4 : ? #6, chr$($2D)
pos.  5,4 : ? #6, chr$($2E) ' queen bottom dark square
pos.  6,4 : ? #6, chr$($2F)

pos.  7,3 : ? #6, chr$($AA)
pos.  8,3 : ? #6, chr$($AB) ' queen top light square
pos.  9,3 : ? #6, chr$($AC)
pos.  7,4 : ? #6, chr$($A7)
pos.  8,4 : ? #6, chr$($A8) ' queen bottom light square
pos.  9,4 : ? #6, chr$($A9)
                          
pos. 10,3 : ? #6, chr$($DB)
pos. 11,3 : ? #6, chr$($DC) ' queen top dark square
pos. 12,3 : ? #6, chr$($DD)
pos. 10,4 : ? #6, chr$($AD)
pos. 11,4 : ? #6, chr$($AE) ' queen bottom dark square
pos. 12,4 : ? #6, chr$($AF)
                         
' King test
pos.  1,5 : ? #6, chr$($5E)
pos.  2,5 : ? #6, chr$($5F) ' king top light square
pos.  3,5 : ? #6, chr$($00)
pos.  1,6 : ? #6, chr$($27)
pos.  2,6 : ? #6, chr$($28) ' king (queen|rook) bottom light square
pos.  3,6 : ? #6, chr$($29)

pos.  4,5 : ? #6, chr$($01)
pos.  5,5 : ? #6, chr$($02) ' king top dark square
pos.  6,5 : ? #6, chr$($03)
pos.  4,6 : ? #6, chr$($2D)
pos.  5,6 : ? #6, chr$($2E) ' king (queen|rook) bottom dark square
pos.  6,6 : ? #6, chr$($2F)

pos.  7,5 : ? #6, chr$($DE)
pos.  8,5 : ? #6, chr$($DF) ' king top light square
pos.  9,5 : ? #6, chr$($80)
pos.  7,6 : ? #6, chr$($A7)
pos.  8,6 : ? #6, chr$($A8) ' king (queen|rook) bottom light square
pos.  9,6 : ? #6, chr$($A9)
                         
pos. 10,5 : ? #6, chr$($81)
pos. 11,5 : ? #6, chr$($82) ' king top dark square
pos. 12,5 : ? #6, chr$($83)
pos. 10,6 : ? #6, chr$($AD)
pos. 11,6 : ? #6, chr$($AE) ' king (queen|rook)q bottom dark square
pos. 12,6 : ? #6, chr$($AF)
                       
' Rook test
pos.  1,7 : ? #6, chr$($21)
pos.  2,7 : ? #6, chr$($04) ' rook top light square
pos.  3,7 : ? #6, chr$($05)
pos.  1,8 : ? #6, chr$($27)
pos.  2,8 : ? #6, chr$($28) ' rook (queen|rook) bottom light square
pos.  3,8 : ? #6, chr$($29)

pos.  4,7 : ? #6, chr$($22)
pos.  5,7 : ? #6, chr$($06) ' rook top dark square
pos.  6,7 : ? #6, chr$($07)
pos.  4,8 : ? #6, chr$($2D)
pos.  5,8 : ? #6, chr$($2E) ' rook (queen|rook) bottom dark square
pos.  6,8 : ? #6, chr$($2F)

pos.  7,7 : ? #6, chr$($21)
pos.  8,7 : ? #6, chr$($84) ' rook top light square
pos.  9,7 : ? #6, chr$($85)
pos.  7,8 : ? #6, chr$($A7)
pos.  8,8 : ? #6, chr$($A8) ' rook (queen|rook) bottom light square
pos.  9,8 : ? #6, chr$($A9)
                        
pos. 10,7 : ? #6, chr$($22)
pos. 11,7 : ? #6, chr$($86) ' rook top dark square
pos. 12,7 : ? #6, chr$($87)
pos. 10,8 : ? #6, chr$($AD)
pos. 11,8 : ? #6, chr$($AE) ' rook (queen|rook)q bottom dark square
pos. 12,8 : ? #6, chr$($AF)

' Bishop test
pos.  1,9 : ? #6, chr$($0B)
pos.  2,9 : ? #6, chr$($0C) ' bishop top light square
pos.  3,9 : ? #6, chr$($0D)
pos.  1,10 : ? #6, chr$($08)
pos.  2,10 : ? #6, chr$($09) ' bishop (bishop|knight) bottom light square
pos.  3,10 : ? #6, chr$($0A)

pos.  4,9 : ? #6, chr$($11)
pos.  5,9 : ? #6, chr$($12) ' bishop top dark square
pos.  6,9 : ? #6, chr$($13)
pos.  4,10 : ? #6, chr$($0E)
pos.  5,10 : ? #6, chr$($0F) ' bishop (bishop|knight) bottom dark square
pos.  6,10 : ? #6, chr$($10)

pos.  7,9 : ? #6, chr$($8B)
pos.  8,9 : ? #6, chr$($8C) ' bishop top light square
pos.  9,9 : ? #6, chr$($8D)
pos.  7,10 : ? #6, chr$($88)
pos.  8,10 : ? #6, chr$($89) ' bishop (bishop|knight) bottom light square
pos.  9,10 : ? #6, chr$($8A)
                          
pos. 10,9 : ? #6, chr$($91)
pos. 11,9 : ? #6, chr$($92) ' bishop top dark square
pos. 12,9 : ? #6, chr$($93)
pos. 10,10 : ? #6, chr$($8E)
pos. 11,10 : ? #6, chr$($8F) ' bishop (bishop|knight)q bottom dark square
pos. 12,10 : ? #6, chr$($90)

' Knight test
pos.  1,11 : ? #6, chr$($16)
pos.  2,11 : ? #6, chr$($17) ' knight top light square
pos.  3,11 : ? #6, chr$($18)
pos.  1,12 : ? #6, chr$($14)
pos.  2,12 : ? #6, chr$($09) ' knight (bishop|knight) bottom light square
pos.  3,12 : ? #6, chr$($15)

pos.  4,11 : ? #6, chr$($1B)
pos.  5,11 : ? #6, chr$($1C) ' knight top dark square
pos.  6,11 : ? #6, chr$($1D)
pos.  4,12 : ? #6, chr$($19)
pos.  5,12 : ? #6, chr$($0F) ' knight (bishop|knight) bottom dark square
pos.  6,12 : ? #6, chr$($1A)

pos.  7,11 : ? #6, chr$($16)
pos.  8,11 : ? #6, chr$($17) ' knight top light square
pos.  9,11 : ? #6, chr$($18)
pos.  7,12 : ? #6, chr$($14)
pos.  8,12 : ? #6, chr$($09) ' knight (bishop|knight) bottom light square
pos.  9,12 : ? #6, chr$($15)
                           
pos. 10,11 : ? #6, chr$($1B)
pos. 11,11 : ? #6, chr$($1C) ' knight top dark square
pos. 12,11 : ? #6, chr$($1D)
pos. 10,12 : ? #6, chr$($19)
pos. 11,12 : ? #6, chr$($0F) ' knight (bishop|knight)q bottom dark square
pos. 12,12 : ? #6, chr$($1A)



' SETUP P/M Graphics
PMGRAPHICS 2   ' DOUBLE LINE MODE
p0addr = PMADR(0)  ' get player addresses
p1addr = PMADR(1)
p2addr = PMADR(2)

MSET p0addr, 128, 0 ' SET all bytes
MSET p1addr, 128, 0 ' SET all bytes
MSET p2addr, 128, 0 ' SET all bytes
'MSET p0addr, 128, $ff ' SET all bytes
'MSET p1addr, 128, $ff ' SET all bytes
'MSET p2addr, 128, $ff ' SET all bytes
'
SETCOLOR -4, 1, 8
SETCOLOR -3, 3, 8
SETCOLOR -2, 5, 8

DATA PMDATA() BYTE= $38,$44,$54,$44,$38

POKE 53256, 3 ' PHAT PLAYERS
POKE 53257, 3
POKE 53258, 3




DO 
  PAUSE ' wait for blank
  PMHPOS 0, 96
  PMHPOS 1, 128
  PMHPOS 2, 160
  MOVE ADR(PMDATA), p0addr +60, 5
  MOVE ADR(PMDATA), p1addr +60, 5
LOOP


