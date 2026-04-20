import os,  argparse, logging
import socket
import threading
import struct


MAX_PLAYERS = 4

# World State: [ID0_X, ID0_Y, ID1_X, ID1_Y, ID2_X, ID2_Y, ID3_X, ID3_Y]
player_data = [0] * (MAX_PLAYERS * 2)
player_slots = [False] * MAX_PLAYERS
lock = threading.Lock()

def get_world_packet():
    """Packets 9 bytes: [Header (0xFF)] + [8 bytes of X/Y data]"""
    print(player_data)
    with lock:
        return struct.pack('>B8B', 0xFF, *player_data)

def handle_client(conn, addr):
    print(f"[*] New connection from {addr}")
    helo = "HELO\n"
    conn.sendall( helo.encode('utf-8') )
    my_id = -1
    
    try:
        while True:
            data = conn.recv(1024)
            if not data:
                break

            cmd = chr(data[0])
            print(f'CMD: {cmd} data {data}  len {len(data)}')
            # JOIN: 'J'
            if cmd == 'J':
                with lock:
                    for i in range(MAX_PLAYERS):
                        if not player_slots[i]:
                            player_slots[i] = True
                            my_id = i
                            break
                response = struct.pack('>B', my_id if my_id != -1 else 0xFE) # 0xFE = Full
                conn.sendall(response)
                print(f"[*] {addr} joined as Player {my_id}")

            # UPDATE: 'U' + X byte + Y byte
            elif cmd == 'U' and my_id != -1:
                if len(data) >= 3:
                    new_x, new_y = data[1], data[2]
                    print(f'x: {new_x} y: {new_y}')
                    # --- AUTHORITATIVE CHECK HERE ---
                    # e.g., if game_map[new_y * 256 + new_x] == WALKABLE:
                    with lock:
                        player_data[my_id*2] = new_x
                        player_data[my_id*2+1] = new_y
                    
                    # Immediate response with world state
                    conn.sendall(get_world_packet())

            # STATE: 'S'
            elif cmd == 'S':
                conn.sendall(get_world_packet())

    except Exception as e:
        print(f"[!] Error with {addr}: {e}")
    finally:
        if my_id != -1:
            with lock:
                player_slots[my_id] = False
                player_data[my_id*2] = 0
                player_data[my_id*2+1] = 0
        conn.close()
        print(f"[*] {addr} disconnected")

def main(args):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1) # Kill Nagle's Lag
    server.bind((args.address, args.port))
    server.listen()
    print(f"[*] Server listening on {args.address}:{args.port}")

    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()

if __name__ == "__main__":
    parser = argparse.ArgumentParser( 
        description = "server for smdt/retro.link networking",
        epilog = "As an alternative to the commandline, params can be placed in a file, one per line, and specified on the commandline like '%(prog)s @params.conf'.",
        fromfile_prefix_chars = '@' )

    parser.add_argument( "-a",
        "--address",
        default = '0.0.0.0',
        help = "Address to listen at",
        metavar = "ARG")

    parser.add_argument( "-p",
        "--port",
        default = 5364,
        type=int,
        help = "Port",
        metavar = "ARG")

    args = parser.parse_args()
    main(args)

