# pip3 install pillow

import sys
from PIL import Image

def save_pixel_data_to_binary(image_path, binary_file_path):
    with Image.open(image_path) as img:
        pixel_data = img.tobytes()
        with open(binary_file_path, 'wb') as binary_file:
            binary_file.write(pixel_data)


if __name__ == "__main__":
	if len(sys.argv) != 2:
		print("Usage: python extract_image_pixels.py <path/to/image>")
		sys.exit(0)

	image_path = sys.argv[1]
	binary_file_path = image_path + ".bin"  # Change this to your desired binary file path

	save_pixel_data_to_binary(image_path, binary_file_path)
	print(f"Pixel data saved to {binary_file_path}")
