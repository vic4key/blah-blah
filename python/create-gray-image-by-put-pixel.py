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

import glob, math
from PIL import Image
from enum import Enum
import PyVutils as vu

class SizeType(Enum):
  FIXED   = 0, # Eg. The file name must be contained (cols x rows)
  SQUARE  = 1,
  INNAME  = 2,

path_dir = RF"data\imgray\*.bin"
cols, rows = None, None
binary = True
size_type = SizeType.INNAME

if size_type is SizeType.FIXED:
  assert rows and cols is not None, "missing value for number of rows and columns"

color_white = 0xFF
color_black = 0x00

for file_path in glob.glob(path_dir):

  f = open(file_path, "rb")
  data = f.read()
  f.close()

  data_size = len(data)
  file_name = vu.extract_file_name(file_path)

  if size_type is SizeType.SQUARE:
    size = int(math.sqrt(data_size))
    rows, cols = size, size
  elif size_type is SizeType.INNAME:
    s = vu.regex(file_name, "([\d]+)x([\d]+)")[0]
    cols, rows = int(s[0]), int(s[1])

  assert rows*cols == data_size, "data size did not met the number of rows and columns"

  im = Image.new("L", (cols, rows), color_black)
  print("image = '%s' (%s)" % (file_name, vu.format_bytes(data_size)))

  for row in range(0, rows):
    for col in range(0, cols):
      pixel = data[row * cols + col]
      if binary: pixel = color_black if pixel == 0 else color_white
      im.putpixel((col, row), pixel)

  im.save(file_path + ".gray.png")
