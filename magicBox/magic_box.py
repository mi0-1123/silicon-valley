import sys
import glob
import random

import pygame.mixer
import serial

from sushi_parameter import SushiParameter

BLUETOOTH = '/dev/ttyAMA0'
#BLUETOOTH = '/dev/ttyS0'
RATE = 9600


if __name__ == '__main__':
    
    ser = serial.Serial(BLUETOOTH, RATE, timeout=2)
    drumroll_list = glob.glob("soundfile/roll/*.mp3")
    complete_sound_list = glob.glob('soundfile/comp/*.mp3')

    while True:
        try:
            is_finished = False
            strmsg=ser.readline().decode()  #受け取ったメッセージをbyte->stringに直してstrmsgに代入してます
            print('get',strmsg,end='')  #Oya機から受け取った値を出力します
            received_values = list(strmsg.split(','))
            sushi_parameter = SushiParameter(received_values[0],received_values[1],received_values[2],received_values[3])
            
            #くっついて縦振りしてると音楽を流す
            if sushi_parameter.is_shake and sushi_parameter.is_stick:
                drumroll_file = random.choice(drumroll_list)
                play_music(drumroll_file)
                is_finished = True
        
            #握っていて、距離が違いと鳴らす
            if is_finished and sushi_parameter.dist > -43.0:
                complete_file = random.choice(complete_sound_list)
                if not pygame.mixer.music.get_busy():
                    play_music(complete_file)



        except KeyboardInterrupt:
            ser.close()
            sys.exit(0)



def play_music(self, filepath):
    if pygame.mixer.get_init() is None:
        pygame.mixer.init()
    
    pygame.mixer.music.load(filepath)
    pygame.mixer.music.play(1)

