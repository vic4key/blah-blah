# nano create-gray-image-by-put-pixel.py & py3 create-gray-image-by-put-pixel.py

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
import glob, math

path_dir = RF"data\imgray\*.bin"
binary = False
square = True
rows, cols = None, None # required if `square = False`

color_white = 0xFF
color_black = 0x00

for file_path in glob.glob(path_dir):

  data = File.Read(file_path)

  if square:
    size = int(math.sqrt(len(data)))
    rows, cols = size, size
  
  im = Image.new("L", (rows, cols), color_black)

  for row in range(0, rows):
    for col in range(0, cols):
      pixel = data[row * cols + col]
      if binary: pixel = color_black if pixel == 0 else color_white
      im.putpixel((col, row), pixel)

  im.save(file_path + ".gray.png")
