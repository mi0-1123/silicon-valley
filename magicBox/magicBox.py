import serial
import time
import sys

BLUETOOTH = '/dev/ttyAMA0'
#BLUETOOTH = '/dev/ttyS0'
RATE = 9600


if __name__ == '__main__':
    ser = serial.Serial(BLUETOOTH, RATE, timeout=2)
    while True:
        try:
            strmsg=ser.readline().decode()
            print('get',strmsg,end='')
        except KeyboardInterrupt:
            ser.close()
            sys.exit(0)



