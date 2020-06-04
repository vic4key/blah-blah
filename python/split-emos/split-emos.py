# -*- coding: utf-8 -*-

'''''''''''''''''''''''''''''''''
@Author : Vic P.
@Email  : vic4key@gmail.com
@Name   : Split Emos
'''''''''''''''''''''''''''''''''

import sys
import numpy as np
from PyVutils import Cv, Others

def main():

	im = Cv.Load("81728769_476823319933844_5792766045886349312_n.jpg")

	nX, nY = (8, 5) # num X/Y icons
	cX, cY = (int(im.shape[1] / nX), int(im.shape[0] / nY)) # size X/Y of an icon

	for Y in range(0, nY):

		emo_stack = None

		for X in range(0, nX):

			# split emo from image
			# emo = im[cY*Y : cY*(Y + 1), cX*X : cX*(X + 1)]
			emo = im[cY*Y : cY*(Y + 1), cX*X + X : cX*(X + 1) + X] # fixed the emo width by padding

			# save emo to file
			Cv.Save(RF"emos\{Y}-{X}.jpg", emo)

			# push emo to stack
			if emo_stack is None: emo_stack = emo
			else: emo_stack = np.hstack((emo_stack, emo))

		# display the emos horizontal stack
		Cv.Display(emo_stack)

	return 0

if __name__ == "__main__":
	try: sys.exit(main())
	except (Exception, KeyboardInterrupt): Others.LogException(sys.exc_info())
