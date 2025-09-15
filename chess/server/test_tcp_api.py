import pytest
import socket
import threading
import time
import socketserver

from rlsf_chess.tcp_api import TcpChessHandler

from rlsf_chess.chess_game import ChessGame


@pytest.fixture
def tcp_server():
    server = socketserver.ThreadingTCPServer(("127.0.0.1", 0), TcpChessHandler)
    host, port = server.server_address

    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    yield host, port
    server.shutdown()
    server.server_close()
    thread.join()


def send_cmd(port, line):
    with socket.create_connection(("127.0.0.1", port), timeout=20) as sock:
        # First line is greeting
        greeting = sock.recv(1024).decode().strip()
        sock.sendall((line + "\r").encode())
        data = sock.recv(1024).decode().strip()
        return greeting, data

def test_new_and_state(tcp_server):
    host, port = tcp_server
    greet, resp = send_cmd(port, "N:S:W\n")
    assert greet.startswith("HELO")
    ids = resp.split()[1]
    assert ids[8] == ":"
    assert ids[:8].isalnum() == True
    assert ids[9:-1].isalnum() == True
    gid = ids[:8] 
    # Query state
    greet, resp = send_cmd(port, f"S:{gid}\n")
    assert "TURN w" in resp
    assert "MVNO 0" in resp

def test_new_SW10(tcp_server):
    host, port = tcp_server
    greet, resp = send_cmd(port, "N:S:W:10\n")
    assert greet.startswith("HELO")
    ids = resp.split()[1]
    assert ids[8] == ":"
    assert ids[:8].isalnum() == True
    assert ids[9:-1].isalnum() == True

def test_joingame(tcp_server):
    host, port = tcp_server
    greet, resp = send_cmd(port, "N:S:W\n")
    assert greet.startswith("HELO")
    ids = resp.split()[1]
    gameid = ids[:8]
    playid = ids[9:-1]

    greet, resp = send_cmd(port, "J:\n")
    assert resp == 'ERR invalid'

    greet, resp = send_cmd(port, "J:AAAAAAAA\n")
    assert resp == 'ERR invalid game id'

    greet, resp = send_cmd(port, f"J:{gameid}\n")
    assert resp == 'ERR invalid mode'

    greet, resp = send_cmd(port, "N:D:W\n")
    assert greet.startswith("HELO")
    ids = resp.split()[1]
    gameid = ids[:8]

    greet, resp = send_cmd(port, f"J:{gameid}\n")
    assert resp.startswith("ACK ")
    pid = resp.split()[1]
    assert pid.isalnum() == True




