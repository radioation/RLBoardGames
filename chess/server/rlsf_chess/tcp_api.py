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
                    mode = parts[1].strip()
                if len(parts) > 2 :
                    player_1_side = parts[2].strip()
                if len(parts) > 3 :
                    level = int(parts[3].strip())

                g = new_game(mode, player_1_side, level )

                return f"ACK {g.id}:{g.player_1_id}\n"
            elif line.startswith("J:"):
                parts = line.split(":")
                print( "AAAAAAA " + str( len(parts ) ) )
                if len(parts) < 2:
                    return "ERR invalid\n"
                gid = parts[1]
                if len(gid) == 0:
                    return "ERR invalid\n"
                game = get_game(gid)
                if game is None: 
                    return "ERR invalid game id\n"
                if game.mode == 'D':
                    return f"ACK {game.player_2_id}\n"
                else:
                    return "ERR invalid mode\n"
            elif line.startswith("M:"):
                parts = line.split(":")
                if len(parts) < 4: return "ERR:bad format"

                gid, pid, uci_move = parts[1], parts[2], parts[3]
                game = get_game(gid)
                movetime_ts = 300
                res = "ACK " + game.do_move( pid, uci_move, movetime_ts)
                return res  
            elif line.startswith("B:"):
                gid = line.split(":",1)[1]
                game = get_game(gid)
                sboard = str(game.board)
                print( sboard )
                
                strboard = "ACK " + re.sub(r"[\n\t\s]*", "", sboard)
                print( strboard )

                return strboard
            elif line.startswith("S:"):
                gid = line.split(":")[1]
                print( gid )
                game = get_game(gid)
                ret = "ACK " +  game.settings_str() + game.state_line()
                return ret
            else:
                return "ERR unknown"
        except Exception as e:
            return f"ERR {e}"


