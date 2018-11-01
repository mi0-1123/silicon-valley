import time
import sys

import serial

from sushi_parameter import SushiParameter

BLUETOOTH = '/dev/ttyAMA0'
#BLUETOOTH = '/dev/ttyS0'
RATE = 9600


if __name__ == '__main__':
    ser = serial.Serial(BLUETOOTH, RATE, timeout=2)
    while True:
        try:
            is_finished = False
            strmsg=ser.readline().decode()  #受け取ったメッセージをbyte->stringに直してstrmsgに代入してます
            print('get',strmsg,end='')  #Oya機から受け取った値を出力します
            received_values = list(strmsg.split(','))
            sushi_parameter = SushiParameter(received_values[0],received_values[1],received_values[2],received_values[3])

            if sushi_parameter.is_shake and sushi_parameter.is_stick:
                print('nyan')
                is_finished = True
        
            if is_finished and sushi_parameter.dist < -43.0:
                print('bow')



        except KeyboardInterrupt:
            ser.close()
            sys.exit(0)



