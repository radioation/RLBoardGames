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

curl -X POST "http://localhost:55557/move" -d "${SESS}"$'\ne2e4\n'
e7e5

curl -X POST "http://localhost:55557/move" -d "${SESS}"$'\ne1e4\n'
illegal move

curl -X POST "http://localhost:55557/move" -d "${SESS}"$'\nd2d3\n'
g8f6

curl -X GET "http://localhost:55557/board?gid=${SESS}"
r n b q k b . r
p p p p . p p p
. . . . . n . .
. . . . p . . .
. . . . P . . .
. . . P . . . .
P P P . . P P P
R N B Q K B N R
```
TCP with telnet

```bash
$ telnet localhost 55558
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
HELO TCP
N:
ACK 3310d05e
M:3310d053:e2e4
ERR 'NoneType' object has no attribute 'do_move'
M:3310d05e:e2e4
e7e5
M:3310d05e:d1d3
illegal move
M:3310d05e:d2d3
g8f6
B:3310d05e
r n b q k b . r
p p p p . p p p
. . . . . n . .
. . . . p . . .
. . . . P . . .
. . . P . . . .
P P P . . P P P
R N B Q K B N R
Connection closed by foreign host.
```

# TODO
* Add check for checkmate/stalemate
* Add two player sessions
* Add SQLite sessions
* Add authentication and authorization for games
* look into FujiNet lobby requirements and see if I can get added.




