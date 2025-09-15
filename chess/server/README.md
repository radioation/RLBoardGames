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
curl -s -X POST localhost:55557/newgame
curl -s -X POST localhost:55557/newgame -d $'D\n' 
curl -X POST "http://localhost:55557/newgame" -d $'D\nB\n'
curl -X POST "http://localhost:55557/newgame" -d $'D\nB\n1\n'
curl -X POST "http://localhost:55557/newgame" -d $'S\nW\n1\n'


ggallard@fedora:~/dev/atari_fastbasic$ curl -X POST "http://localhost:55557/newgame" -d $'S\nW\n1\n'
fd77aff0:e3061270
ggallard@fedora:~/dev/atari_fastbasic$ curl -X POST "http://localhost:55557/move" -d $'fd77aff0\ne3061270\ne2e4\n'
e7e6
ggallard@fedora:~/dev/atari_fastbasic$ curl -X POST "http://localhost:55557/move" -d $'fd77aff0\ne3061270\ne1e4\n'
illegal move
ggallard@fedora:~/dev/atari_fastbasic$ curl -X POST "http://localhost:55557/move" -d $'fd77aff0\ne3061270\nd2d3\n'
d7d5
ggallard@fedora:~/dev/atari_fastbasic$ curl -X GET "http://localhost:55557/board?gid=fd77aff0"
r n b q k b n r
p p p . . p p p
. . . . p . . .
. . . p . . . .
. . . . P . . .
. . . P . . . .
P P P . . P P P
R N B Q K B N R
ggallard@fedora:~/dev/atari_fastbasic$


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




