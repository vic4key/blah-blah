# nano create-image-by-put-pixel.py & py3 create-image-by-put-pixel.py

import random as rd
from PIL import Image

color_white = (0xFF, 0xFF, 0xFF)
color_black = (0x00, 0x00, 0x00)

size = rows, cols = 300, 300

im = Image.new("RGB", size, color_white)

for row in range(0, rows):
  for col in range(0, cols):
    if rd.randrange(255) % 2 == 0:
      im.putpixel((col, row), color_black)

im.show()
