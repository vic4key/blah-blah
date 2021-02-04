# nano create-rgb-image-by-put-pixel.py & py3 create-rgb-image-by-put-pixel.py

'''
      {
        std::string s = "path\\to\\image.bin";
        auto f = fopen(s.c_str(), "wb");
        fwrite(&image(0), 1, image.bytes(), f);
        fclose(f);
      }
'''

import random as rd
from PIL import Image
from PyVutils import File
from random import *
import glob

# input parameter
path_dir = RF"data\imgray\*.bin"
rows, cols = 801, 801 # input parameter

color_white = (0xFF, 0xFF, 0xFF)
color_black = (0x00, 0x00, 0x00)

for file_path in glob.glob(path_dir):

  data = File.Read(file_path)

  rows, cols = 801, 801
  im = Image.new("RGB", (rows, cols), color_white)

  for row in range(0, rows):
    for col in range(0, cols):
      # pixel = data[row * cols + col] # 3 bytes
      r, g, b = (randint(0x00, 0xFF), randint(0x00, 0xFF), randint(0x00, 0xFF))
      color = (r, g, b)
      im.putpixel((col, row), color)

  im.save(file_path + ".rgb.png")
