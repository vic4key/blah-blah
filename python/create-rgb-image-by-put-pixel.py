# nano create-image-by-put-pixel.py & py3 create-image-by-put-pixel.py

'''
      {
        std::string s = "T:\\test\\image.bin";
        auto f = fopen(s.c_str(), "wb");
        fwrite(&image(0), 1, image.bytes(), f);
        fclose(f);
      }
'''

import random as rd
from PIL import Image
from PyVutils import File
import glob

color_white = (0xFF, 0xFF, 0xFF)
color_black = (0x00, 0x00, 0x00)

for file_path in glob.glob(RF"T:\test\*.bin"):

  data = File.Read(file_path) # RF"T:\test\{name}.bin")

  rows, cols = 801, 801
  im = Image.new("RGB", (rows, cols), color_white)

  for row in range(0, rows):
    for col in range(0, cols):
      if data[row * cols + col] % 2 == 0:
        im.putpixel((col, row), color_black)

  im.save(file_path + ".png")
