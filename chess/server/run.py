# run_both.py
import threading, socketserver
from rlsf_chess.http_api import app
from rlsf_chess.tcp_api import TcpChessHandler

def start_tcp():
    server = socketserver.ThreadingTCPServer(("0.0.0.0", 55558), TcpChessHandler)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    print("TCP listening on :55558")

if __name__ == "__main__":
    start_tcp()
    app.run(host="0.0.0.0", port=55557)

