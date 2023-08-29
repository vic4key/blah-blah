# https://rawpixels.net/

'''
      {
        std::string s = "path\\to\\bla-bla\\python\\data\\imrgb\\"
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

# input parameter
path_dir = RF"data\imrgb\*.bin"
cols, rows = None, None # required if `square = False`
size_type = SizeType.INNAME

if size_type is SizeType.FIXED:
  assert rows and cols is not None, "missing value for number of rows and columns"

color_white = (0xFF, 0xFF, 0xFF)
color_black = (0x00, 0x00, 0x00)

for file_path in glob.glob(path_dir):

  f = open(file_path, "rb")
  data = f.read()
  f.close()

  data_size = len(data)
  file_name = vu.extract_file_name(file_path)
  print(data_size)

  if size_type is SizeType.SQUARE:
    size = int(math.sqrt(data_size))
    rows, cols = size, size
  elif size_type is SizeType.INNAME:
    s = vu.regex(file_name, "([\d]+)x([\d]+)")[0]
    cols, rows = int(s[0]), int(s[1])

  assert rows*cols*3 == data_size, "data size (%dx%d) did not met number of rows and columns" % (rows, cols)

  tmp = Image.frombytes("RGB", (cols, rows), data)

  img = Image.new("RGB", (rows, cols), color_white)
  print("image = '%s' (%s)" % (file_name, vu.format_bytes(data_size)))

  for row in range(0, rows):
    for col in range(0, cols):
      r, g, b = tmp.getpixel((col, row))
      pixel = (r, g, b)
      img.putpixel((col, row), pixel)

  img.save(file_path + ".png")
