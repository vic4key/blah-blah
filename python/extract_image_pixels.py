# pip3 install pillow

import os, sys
from PIL import Image

def extract_and_save_pixels_to_binary(image_path, binary_file_path):
    with Image.open(image_path) as img:
        pixel_data = img.tobytes()
        with open(binary_file_path, "wb") as f:
            f.write(pixel_data)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python extract_image_pixels.py <path/to/image>")
        sys.exit(0)

    image_path  = sys.argv[1]
    binary_path = os.path.splitext(image_path)[0] + ".bin"

    extract_and_save_pixels_to_binary(image_path, binary_path)
    print(f"the image pixels saved to '{binary_path}'")