# This is a sample Python script.
import math
import numpy as np
import sys
import pcl
import pcl
import socket
import struct
import matplotlib.pyplot as plt
if __name__ == '__main__':

    # Create socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind socket to local address
    sock.bind(('127.0.0.1', 12345))

    # Listen for incoming connections
    sock.listen(1)

    # Accept connection
    conn, addr = sock.accept()
    print(f"Client connected from {addr[0]}:{addr[1]}")

    # Receive data
    rows = 2000
    cols = 2000
    received = b''
    data_in_size = rows*cols
    while(len(received) <= data_in_size):
        data = conn.recv(data_in_size - len(received))
        if not data:
            break
        received += data
    # unpack the data as a 2D array of bools
    bool_array = struct.unpack(f'{rows*cols}B', received)
    bool_array = [bool(b) for b in bool_array]
    bool_array = [bool_array[i:i + rows] for i in range(0, len(bool_array), rows)]
    for values in bool_array:
        if values == True:
            print(values)
    plt.figure(1)
    plt.imshow(bool_array, interpolation='nearest')
    plt.grid(True)
    plt.show()
    conn.close()