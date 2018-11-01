import pygame.mixer
import serial
import time
import sys
import glob
import random

BLUETOOTH = '/dev/ttyAMA0'
#BLUETOOTH = '/dev/ttyS0'
RATE = 9600


if __name__ == '__main__':

	myserial = serial.Serial(BLUETOOTH, RATE, timeout=1)
	mplist = glob.glob("soundfile/comp/*.mp3")
	rollist = glob.glob("soundfile/roll/*.mp3")
	pygame.mixer.init()
	num = len(mplist)
	rollen = len(rollist)
	count = 0
	flg=False

	while True:
		try:
			print('count',count)
			count += 1

			strmsg=myserial.readline()
			print('get',strmsg)
			if len(strmsg) is not 0:
				vallist = strmsg.split(',')
				vallistint=[]
				for i in vallist:
					tmp1 = i.replace('\n','')
					tmp2 = tmp1.replace('\r','')
					vallistint.append(int(tmp2))

				print('castint',vallistint)

				if  vallistint[2]:
					if	not pygame.mixer.music.get_busy():
							th = random.choice(range(rollen))
							pygame.mixer.music.load(rollist[th])
							pygame.mixer.music.play()
							flg = True
							itr = random.choice(range(num))
				if flg and vallistint[3] > -43:
					pygame.mixer.music.load(mplist[itr])
					pygame.mixer.music.play()
					flg = False

		except KeyboardInterrupt:
			sys.exit(0)
