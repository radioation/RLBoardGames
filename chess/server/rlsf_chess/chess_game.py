import os
import uuid
import chess
import chess.engine

import threading


class ChessGame:
    def __init__(self, mode, player_1_side):
        self.id = str(uuid.uuid4())[:8]
        self.board = chess.Board()      
        self.moves = []                 # list of UCI moves
        self.mode = mode                # single player 'S' or double player 'D'
        self.player_1_side = player_1_side  # for single player, which side is the player
                                        # that created the game: 'W', 'B'
        self.player_1_id = str(uuid.uuid4())[:8]
        self.player_2_id = str(uuid.uuid4())[:8]
        self.curr_player = 1
        if self.mode == 'D' and player_1_side == 'B':
            self.curr_player = 2


    def do_move( self, pid, uci, movetime_ms ):
        if self.curr_player == 1 and pid != self.player_1_id:
            return "illegal move: player 1 turn"
        if self.curr_player == 2 and pid != self.player_2_id:
            return "illegal move: player 2 turn"
            
        try:
             mv = chess.Move.from_uci(uci)
        except ValueError:
            return "illegal move"
        if mv not in self.board.legal_moves:
            return "illegal move"
        self.board.push(mv)
        # get reply from stockfish.
        with chess.engine.SimpleEngine.popen_uci(os.environ['ENGINE_PATH']) as eng:
            res = eng.play(self.board, chess.engine.Limit(time=movetime_ms/1000.0))
            if res.move is None:
                # No legal engine reply (mate/stalemate)
                return "none"
            best = res.move.uci()
            self.curr_player = 2 if self.curr_player == 1 else 1
            print("BEST: " +best)
            if self.mode == 'S':
                self.board.push(res.move)
            return best
 
    def state_line(self):
        return f"TURN {'w' if self.board.turn else 'b'} MVNO {len(self.moves)} LAST {self.moves[-1] if self.moves else '-'}"


GAMES = {}
GAMES_LOCK = threading.Lock()

def new_game(mode,side) -> ChessGame:
    g = ChessGame(mode,side)
    GAMES[g.id] = g
    return g

def get_game(gid: str) -> ChessGame | None:
    return GAMES.get(gid)
