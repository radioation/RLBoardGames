import os
from flask import Flask, request, Response

from chess_game import new_game, get_game

#######################################################
#
# Flask HTTP interface
#
app = Flask(__name__)

@app.post("/newgame")
def http_newgame():
    g = new_game()
    return Response(g.id + "\n", mimetype="text/plain")

@app.post("/move")
def move():
    """
    Body format (plain text, LF line endings):
        <game_id>\n
        <uci_move>\n
        [<movetime_ms>]\n        # optional, default 300
    Returns:
        - "invalid move\n"           if POST body is invalid.
        - "illegal move\n"           if move is not legal.
        - "<bestmove>\n"             if valid, give best move from stockfish (single player will move)
    """
    body = request.get_data(as_text=True) or ""

    print ("BODY: " + body)
    lines = [ln.strip() for ln in body.splitlines() if ln.strip() != ""]

    # Not enough info (we need the id and the UCI move or things can't work
    if len(lines) < 2:
        return Response("invalid format\n", mimetype="text/plain", status=400)
   
    gid, uci_move = lines[0], lines[1].lower()
    if not gid.isalnum() or len(gid) > 8:
        return Response("invalid format\n", mimetype="text/plain", status=400)
    if not uci_move.isalnum() or len(uci_move) > 5:
        return Response("invalid format\n", mimetype="text/plain", status=400)

    movetime_ms = 300
    if len(lines) >= 3 and lines[2].isdigit():
        movetime_ms = int(lines[2])

    game = get_game(gid)
    if game is None:
        return Response("illegal move\n", mimetype="text/plain", status=404)

    return Response( game.apply_move( uci_move ) + "\n", mimetype="text/plain", status=400)



@app.get("/board")
def board():
    gid = request.args.get('gid')
    game = get_game(gid)
    if not game: return Response("ERR no game\n", mimetype="text/plain", status = 404 )

    return Response(str(game.board) + "\n", mimetype="text/plain")

if __name__ == "__main__":
    # Run it:  python3 chess_server.py
    app.run(host="0.0.0.0", port=5555)

