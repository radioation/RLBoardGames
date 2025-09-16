#  Usage

needs a UCI engine for single player

```bash
pip3 install flask python-chess

ENGINE_PATH = "/path/to/stockfish/stockfish-ubuntu-x86-64-avx2"

```
Run with 
```bash
python3 run.py
```


# Trying the interface 

HTTP with CURL
```bash

$  curl -X POST "http://localhost:55557/newgame" -d $'D\nW\n1\n'
9f8765a0:d2c9f9ba
$  curl -X POST "http://localhost:55557/joingame" -d $'9f8765a0\n'
3c8c6a97
$ curl -X POST "http://localhost:55557/move" -d $'9f8765a0\n3c8c6a97\ne2ef\n'
illegal move: player 1 turn
$ curl -X POST "http://localhost:55557/move" -d $'9f8765a0\nd2c9f9ba\ne2ef\n'
illegal move
$ curl -X POST "http://localhost:55557/move" -d $'9f8765a0\nd2c9f9ba\ne2e4\n'
c7c5
$ curl -X POST "http://localhost:55557/move" -d $'9f8765a0\nd2c9f9ba\ne7e5\n'
illegal move: player 2 turn
$ curl -X POST "http://localhost:55557/move" -d $'9f8765a0\n3c8c6a97\ne7e5\n'
d2d4
$ curl -X GET "http://localhost:55557/status?gid=9f8765a0"
TURN w:MVNO 2:LAST e7e5
$




$ curl -X POST "http://localhost:55557/newgame" -d $'S\nW\n1\n'
fd77aff0:e3061270
$ curl -X POST "http://localhost:55557/move" -d $'fd77aff0\ne3061270\ne2e4\n'
e7e6
$ curl -X POST "http://localhost:55557/move" -d $'fd77aff0\ne3061270\ne1e4\n'
illegal move
$ curl -X POST "http://localhost:55557/move" -d $'fd77aff0\ne3061270\nd2d3\n'
d7d5
$ curl -X GET "http://localhost:55557/board?gid=fd77aff0"
r n b q k b n r
p p p . . p p p
. . . . p . . .
. . . p . . . .
. . . . P . . .
. . . P . . . .
P P P . . P P P
R N B Q K B N R
ggallard@fedora:~/dev/atari_fastbasic$

 curl -X GET "http://localhost:55557/status?gid=6918d8bc"

```
TCP with telnet

```bash

ggallard@fedora:~/dev/RLBoardGames/chess/server$ telnet 10.25.50.61 55558
Trying 10.25.50.61...
Connected to 10.25.50.61.
Escape character is '^]'.
HELO
N:S:W:1
ACK e78c2852:b6dc3dda

S:e78c2852
ACK mode:S p1side:W level:1 curr_player:1
TURN w MVNO 0 LAST -
M:e78c2852:b6dc3dda
ERR:bad format
M:e78c2852:b6dc3dda:d2d3
ACK d7d5
S:e78c2852
ACK mode:S p1side:W level:1 curr_player:1

TURN w MVNO 0 LAST -
M:e78c2852:b6dc3dda:d1d3
ACK illegal move
M:e78c2852:b6dc3dda:e2e4
ACK e7e6
B:e78c2852
ACK rnbqkbnrppp..ppp....p......p........P......P....PPP..PPPRNBQKBNR

```

# TODO
* Add check for checkmate/stalemate
* Add two player sessions
* Add SQLite sessions
* Add authentication and authorization for games
* look into FujiNet lobby requirements and see if I can get added.




