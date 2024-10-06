import socket

def main():
    host = '127.0.0.1'
    port = 5000

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        message = "Hello, server!"
        s.sendall(message.encode())
        data = s.recv(1024)
        print(f"Sent: {message} as {message.encode()}") 

    print(f"Received: {data.decode()}")

if __name__ == "__main__":
    main()
