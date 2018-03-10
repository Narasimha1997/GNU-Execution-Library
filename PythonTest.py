'''Creates and executes 3 sockets, sending data to each socket at a time, 
in round robin mode'''

import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    sock.connect(('localhost', 6000))
    sock.send(bytes('Hello '.encode()))

    sock.connect(('localhost', 6001))
    sock.send(bytes('Hello '.encode()))
    
    sock.connect(('localhost', 6002))
    sock.send(bytes('Hello '.encode()))
    
    print('Round completed')
    time.sleep(2)

    