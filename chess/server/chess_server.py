import os
import uuid
import chess
import chess.engine
from flask import Flask, request, Response

app = Flask(__name__)

# In-memory sessions: game_id -> chess.Board
SESSIONS = {}

def new_board():
    return chess.Board()

@app.post("/newgame")
def newgame():
    gid = str(uuid.uuid4())[:8]
    SESSIONS[gid] = new_board()
    return Response(gid + "\n", mimetype="text/plain")

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
   
    gid, uci = lines[0], lines[1].lower()
    if not gid.isalnum() or len(gid) > 8:
        return Response("invalid format\n", mimetype="text/plain", status=400)
    if not uci.isalnum() or len(uci) > 5:
        return Response("invalid format\n", mimetype="text/plain", status=400)

    movetime_ms = 300
    if len(lines) >= 3 and lines[2].isdigit():
        movetime_ms = int(lines[2])

    board = SESSIONS.get(gid)
    if board is None:
        return Response("illegal move\n", mimetype="text/plain", status=404)

    # check move against their list of moves with python-chess
    try:
        mv = chess.Move.from_uci(uci)
    except ValueError:
        return Response("illegal move\n", mimetype="text/plain", status=400)

    if mv not in board.legal_moves:
        return Response("illegal move\n", mimetype="text/plain", status=400)

    board.push(mv)

    # get reply from stockfish.
    with chess.engine.SimpleEngine.popen_uci(os.environ['ENGINE_PATH']) as eng:
        res = eng.play(board, chess.engine.Limit(time=movetime_ms/1000.0))
        if res.move is None:
            # No legal engine reply (mate/stalemate)
            return Response("(none)\n", mimetype="text/plain")
        best = res.move.uci()
        print("BEST: " +best)
        board.push(res.move)
        return Response(best + "\n", mimetype="text/plain")

@app.get("/board")
def board():
    gid = request.args.get('gid')
    board = SESSIONS.get(gid)
    return Response(str(board) + "\n", mimetype="text/plain")

if __name__ == "__main__":
    # Run it:  python3 chess_server.py
    app.run(host="0.0.0.0", port=5555)

