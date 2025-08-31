import socket

HOST = '0.0.0.0'  
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Listening on {HOST}:{PORT}")
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            data = conn.recv(1024)  # Receive up to 1024 bytes
            print( len(data) )
            if not data:
                break  # No more data, connection closed by client
            conn.sendall(data) # Echo back the received data
            print(f"Echoed: {data.decode()}") # Decode for printing
