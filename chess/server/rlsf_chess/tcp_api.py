# tcp_protocol.py
import socketserver, threading
from rlsf_chess.chess_game import new_game, get_game
import re


class TcpChessHandler(socketserver.StreamRequestHandler):
    def handle(self):
        print("connected")
        self.wfile.write(b"HELO\n")
        for line in self.rfile:
            print("pre-strip: ", end="" )
            print(line)
            line = line.decode("utf-8").strip()
            print("POST-strip: " + line)
            if not line:
                continue
            response = self.dispatch(line)
            print(response)
            self.wfile.write((response + "\n").encode("utf-8"))

    def dispatch(self, line: str) -> str:
        try:
            if line.startswith("N:"):
                mode = 'S'
                player_1_side = 'W'
                level = 3
                parts = line.split(":")
                if len(parts) > 1 :
                    mode = len[1].strip()
                if len(parts) > 2 :
                    mode = len[2].strip()
                if len(parts) > 3 :
                    level = len[3].strip()

                g = new_game(mode, player_1_side, level )
                return f"ACK {g.id}:{g.player_1_id}"
            elif line.startswith("J:"):
                gid = line.split(":",1)[1]
                return "NAK "
                #side, tok = join_game(gid)
                #return f"JOINOK {side} {tok}"
            elif line.startswith("M:"):
                parts = line.split(":",2)
                if len(parts) < 3: return "ERR:bad format"
                gid, uci_move = parts[1], parts[2]
                game = get_game(gid)
                movetime_ts = 300
                res = "ACK " + game.do_move( uci_move, movetime_ts)
                return res  
            elif line.startswith("B:"):
                gid = line.split(":",1)[1]
                game = get_game(gid)
                sboard = str(game.board)
                print( sboard )
                
                strboard = "ACK " + re.sub(r"[\n\t\s]*", "", sboard)
                print( strboard )

                return strboard
            else:
                return "ERR unknown"
        except Exception as e:
            return f"ERR {e}"


