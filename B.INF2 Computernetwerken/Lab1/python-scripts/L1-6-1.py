#!/usr/bin/env python3
import socket

host = "www.google.com"
port = 80

request = (
    "GET / HTTP/1.1\r\n"
    f"Host: {host}\r\n"
    "Connection: close\r\n"
    "\r\n"
)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((host, port))

client.send(request.encode())

response = b""
while True:
    chunk = client.recv(4096)
    if not chunk:
        break
    response += chunk

print(response.decode(errors='ignore'))
client.close()
