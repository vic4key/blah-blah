# nano create-gray-image-by-put-pixel.py & py3 create-gray-image-by-put-pixel.py

'''
      {
        std::string s = "path\\to\\bla-bla\\python\\data\\imgray\\";
        s += "image";
        s += ".bin";
        auto f = fopen(s.c_str(), "wb");
        fwrite(&image(0), 1, image.bytes(), f);
        fclose(f);
      }
'''

import random as rd
from PIL import Image
import glob, math

path_dir = RF"data\imgray\*.bin"
binary = True
cols, rows = None, None
square = cols == rows

if not square:
  assert rows and cols is not None, "missing value for number of rows and columns"

color_white = 0xFF
color_black = 0x00

for file_path in glob.glob(path_dir):

  f = open(file_path, "rb")
  data = f.read()
  f.close()

  if square:
    size = int(math.sqrt(len(data)))
    rows, cols = size, size

  assert rows*cols == len(data), "data size did not met number of rows and columns"

  im = Image.new("L", (cols, rows), color_black)
  print("image =", im, "<%d>" % len(data))

  for row in range(0, rows):
    for col in range(0, cols):
      pixel = data[row * cols + col]
      if binary: pixel = color_black if pixel == 0 else color_white
      im.putpixel((col, row), pixel)

  im.save(file_path + ".gray.png")
