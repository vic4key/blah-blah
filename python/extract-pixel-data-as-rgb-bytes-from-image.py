# https://rawpixels.net/

import numpy as np
from PIL import Image
import PyVutils as vu

file_path = R"path\to\image"
file_dir  = vu.extract_file_directory(file_path, False)
file_name = vu.extract_file_name(file_path, False)

# read as rgba image
img = Image.open(file_path)
img = img.convert("RGBA")
dat = np.array(img)

# extract r-g-b-a channels
red, green, blue, alpha = dat.T 

# create rgb bytes from r-g-b channels
dat = np.array([blue, green, red])
dat = dat.transpose()
img = Image.fromarray(dat)
dat = img.tobytes()

# save rgb bytes as binary file
file_path = "%s\\%s_%dx%d.bin" % (file_dir, file_name, img.size[0], img.size[1])
print(file_path)
vu.write_file(file_path, dat)