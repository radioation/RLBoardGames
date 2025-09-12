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


# TODO
* Add check for checkmate/stalemate
* Add two player sessions
* Add SQLite sessions
* Add authentication and authorization for games
* look into FujiNet lobby requirements and see if I can get added.




