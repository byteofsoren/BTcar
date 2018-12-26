import serial
import struct
import subprocess
from bitarray import bitarray
from time import sleep

def find_port():
    return subprocess.getstatusoutput("pio device list | grep ttyACM | grep dev")[1]
def is_bit_set(nr: int, bitnr:int):
    if nr & (1 << bitnr):
        return True
    return False


class Flags(object):
    """Manages the flags variable"""
    def __init__(self):
        setFlags(0,0,0)

    def setFlags(self, dis_connect, acc, system_enable):
        if system_enable == True:
            self.system_enable= 1
        else:
            self.system_enable= 0

        if acc == True:
            self.acc = 1
        else:
            self.acc = 0

        if dis_connect == True:
            self.dis_connect = 1
        else:
            self.dis_connect = 0

        if system_enable == True:
            self.system_enable = 1
        else:
            self.system_enable = 0

    def encode(self):
        ret = 0
        ret += 1* self.system_enable
        ret += 2* self.acc
        ret += 4* self.dis_connect
        return struct.pack('>1B', ret)

    def decode(self, flags):
        temp = struct.unpack('>B', flags)
        bits = bitarray(temp)
        setFlags(bits[2],bits[1],bits[0])
        return bits[0:2]






class Car(object):
    """Creates an comunication inteface for the car"""
    def __init__(self, port):
        super(car, self).__init__()
        #self.serial = serial.Serial(self.port)
        self.flags = Flags()

    def connect(self):
        while (self.serial.inWaiting() <= 1):
            print("Waiting for connection")
        # Wrtite to enable to Arduino
        self.serial.write(struct.pack('>B', 1))





