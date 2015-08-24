import socket
import sys
import random

randin = open("/dev/urandom", "r")

while True:
    for i in range(0, 2048):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        server_address = ('localhost', 10000)
        print 'connecting to %s port %s' % server_address
        sock.connect(server_address)

        try:
            name = randin.read(i)
            number = "%s" % random.getrandbits(64)

            sock.sendall('create /%s %s\n' % (name, number))
            data = sock.recv(3)
            print 'received "%s"' % data

            sock.sendall('store /%s\n' % name)
            data = sock.recv(3)
            print 'received "%s"' % data
        except socket.error:
            pass

        finally:
            print 'closing socket'
            sock.close()
