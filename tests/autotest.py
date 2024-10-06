import unittest
import socket
import time
import subprocess

class TestTCPServer(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.server_process = subprocess.Popen(['../build/tcp_server'])
        time.sleep(1)

    @classmethod
    def tearDownClass(cls):
        cls.server_process.terminate()

    def test_echo(self):
        host = '127.0.0.1'
        port = 5000
        message = "Привет, мир!"

        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((host, port))
            s.sendall(message.encode())
            data = s.recv(1024)

        self.assertEqual(data.decode(), message)

    def test_multiple_connections(self):
        host = '127.0.0.1'
        port = 5000
        messages = ["Привет, сервер!", "Пиу пяу я пуляюсь коннектами", "Как дела, сервер?", "Купил мужик шляпу", "А она ему как раз"]

        for message in messages:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((host, port))
                s.sendall(message.encode())
                data = s.recv(1024)

            self.assertEqual(data.decode(), message)

if __name__ == '__main__':
    unittest.main()
