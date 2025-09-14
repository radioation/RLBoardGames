import os
from flask import Flask, request, Response

from rlsf_chess.chess_game import new_game, get_game

#######################################################
#
# Flask HTTP interface
#
app = Flask(__name__)

@app.post("/newgame")
def http_newgame():
    body = request.get_data(as_text=True) or ""
    print ("BODY: " + body)
    mode = 'S'
    player_1_side = 'W'
    lines = [ln.strip() for ln in body.splitlines() if ln.strip() != ""]
    print(len(lines))
    if len(lines) > 0:
        # first line shoudl be mode.
        if lines[0] == 'D':
            mode = 'D'
        if len(lines) > 1:
            if lines[1] == 'B':
                player_1_side = 'B'
 
    g = new_game(mode, player_1_side)
    print( f"new game: {g.id} mode: {g.mode} side: {g.player_1_side}")
    print( f"   p1: {g.player_1_id} p2: {g.player_2_id}")
    return Response(g.id + ":" + g.player_1_id + "\n", mimetype="text/plain")

@app.post("/joingame")
def http_joingame():
    body = request.get_data(as_text=True) or ""
    print ("BODY: " + body)
    lines = [ln.strip() for ln in body.splitlines() if ln.strip() != ""]
    if len(lines) > 0 :
        gid = lines[0]
        game = get_game(gid)
        if game is None:
            return Response("invalid gid\n", mimetype="text/plain", status=404)
        else:
            return Response( game.player_2_id + "\n" )
    else:
        return Response("invalid\n", mimetype="text/plain", status=400)
@app.post("/move")
def http_move():
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

    # Not enough info (we need both ids and the UCI move )
    if len(lines) < 3:
        return Response("invalid format\n", mimetype="text/plain", status=400)
   
    gid, pid,  uci_move = lines[0], lines[1], lines[2].lower()
    if not gid.isalnum() or len(gid) > 8:
        return Response("invalid format\n", mimetype="text/plain", status=400)
    if not pid.isalnum() or len(gid) > 8:
        return Response("invalid format\n", mimetype="text/plain", status=400)
    if not uci_move.isalnum() or len(uci_move) > 5:
        return Response("invalid format\n", mimetype="text/plain", status=400)

    movetime_ms = 300
    if len(lines) >= 4 and lines[3].isdigit():
        movetime_ms = int(lines[3])

    game = get_game(gid)
    if game is None:
        return Response("illegal move\n", mimetype="text/plain", status=404)

    return Response( game.do_move( uci_move, movetime_ms ) + "\n", mimetype="text/plain", status=400)



@app.get("/board")
def http_board():
    gid = request.args.get('gid')
    game = get_game(gid)
    if not game: return Response("ERR no game\n", mimetype="text/plain", status = 404 )

    return Response(str(game.board) + "\n", mimetype="text/plain")


@app.get("/status")
def http_status():
    gid = request.args.get('gid')
    game = get_game(gid)
    if not game: return Response("ERR no game\n", mimetype="text/plain", status = 404 )

    return Response(str(game.board) + "\n", mimetype="text/plain")






