# tcp_protocol.py
import socketserver, threading
from chess_game import new_game, get_game



class TcpChessHandler(socketserver.StreamRequestHandler):
    def handle(self):
        self.wfile.write(b"HELLO TCP\n")
        for line in self.rfile:
            line = line.decode("utf-8").strip()
            if not line:
                continue
            response = self.dispatch(line)
            self.wfile.write((response + "\n").encode("utf-8"))

    def dispatch(self, line: str) -> str:
        try:
            if line.startswith("NEW:"):
                g = new_game()
                return f"NEWOK {g.id}"
            elif line.startswith("JOIN:"):
                gid = line.split(":",1)[1]
                return "JOINNO "
                #side, tok = join_game(gid)
                #return f"JOINOK {side} {tok}"
            elif line.startswith("MOVE:"):
                parts = line.split(":",2)
                if len(parts) < 3: return "ERR bad format"
                gid, uci_move = parts[1], parts[2]
                game = get_game(gid)
                res = game.apply_move( uci_move)
                return res  
            elif line.startswith("BOARD:"):
                gid = line.split(":",1)[1]
                game = get_game(gid)
                return game.board
            else:
                return "ERR unknown"
        except Exception as e:
            return f"ERR {e}"

def start_tcp_server(host="0.0.0.0", port=6000):
    server = socketserver.ThreadingTCPServer((host, port), RetroHandler)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    print(f"TCP RetroLink server listening on {host}:{port}")
    return server

