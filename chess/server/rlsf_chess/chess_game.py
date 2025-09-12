import uuid
import chess
import chess.engine


class ChessGame:
    def __init__(self):
        self.gid = str(uuid.uuid4())[:8]
        self.board = chess.Board()
        self.moves = []

    def do_move( self, uci, movetime_ms ):
        try:
             mv = chess.Move.from_uci(uci)
        except ValueError:
            return "illegal move"
        if mv not in board.legal_moves:
            return "illegal move"
        board.push(mv)
        # get reply from stockfish.
        with chess.engine.SimpleEngine.popen_uci(os.environ['ENGINE_PATH']) as eng:
            res = eng.play(board, chess.engine.Limit(time=movetime_ms/1000.0))
            if res.move is None:
                # No legal engine reply (mate/stalemate)
                return "none"
            best = res.move.uci()
            print("BEST: " +best)
            board.push(res.move)
            return best
 
    def state_line(self):
        return f"TURN {'w' if self.board.turn else 'b'} MVNO {len(self.moves)} LAST {self.moves[-1] if self.moves else '-'}"


GAMES = {}

def new_game() -> Game:
    g = Game()
    GAMES[g.id] = g
    return g

def get_game(gid: str) -> Game | None:
    return GAMES.get(gid)
