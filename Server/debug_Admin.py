import socket
import select
from time import sleep

# Connection
#HOST='192.168.58.127'
#HOST='10.0.0.1'
HOST='127.0.0.1'
#HOST='10.7.0.109'
PORT=9988
conn = None
addr = None

print("Client en mode Admin")
s= socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

#data = s.recv(1500)

#print(data)
b = bytearray(10)
for i in range(1, 10) :
    b[i-1] = i
b[9] = ord('\n')
s.send(b)

while(True) :
    ready = select.select([s], [], [], 1)
    if ready[0] :
        data = s.recv(1500)
        print(data.decode(encoding='UTF-8'))
    string = input("=>$ ")
    if string != "" :
        s.send((string+'\0').encode())

s.shutdown(socket.SHUT_RDWR)

