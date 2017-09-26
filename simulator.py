#!/usr/bin/python
import argparse
import binascii
import os
import time
import socket
import sys

parser = argparse.ArgumentParser(description='Simulator for sending rcd data.')
parser.add_argument('rcdfile', type=argparse.FileType('rb'),
                    help='source rcd file name')
parser.add_argument('destip', type=str,
                    help='ip address for sending')
parser.add_argument('port', type=int,
                    help='udp port for sending')

args = parser.parse_args()

f = args.rcdfile
ip = args.destip
port = args.port

print "starting to send data to %s:%s" % (ip, port)
print

bytes = bytearray()

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP

endflag = 0

while f:
    #sys.stdout.write("%s " % binascii.b2a_hex(args.rcdfile.read(1)))
    #sys.stdout.flush()
    data = f.read(1)
    bytes.append(data)
    #sys.stdout.write("%s " % binascii.b2a_hex(data))
    #sys.stdout.flush()
    if data == "\x45" :
        endflag = 1
    elif data == "\x4e" and endflag == 1 :
        endflag = 2
    elif data == "\x44" and endflag == 2 :
        endflag = 3
    elif data == "\x21" and endflag == 3 :
        endflag = 4
    else :
        endflag = 0
    if endflag == 4 :
        sock.sendto(bytes, (ip, port))
        bytes = bytearray()
        endflag = 0
        time.sleep(3)
    #time.sleep(1)
f.close
sock.close
