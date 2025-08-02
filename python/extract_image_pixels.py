# pip3 install pillow

import os, argparse
from PIL import Image

def extract_and_save_pixels_to_binary(image_path, binary_file_path):
    with Image.open(image_path) as img:
        pixel_data = img.tobytes()
        with open(binary_file_path, "wb") as f:
            f.write(pixel_data)

def generate_output_filename(image_path):
    with Image.open(image_path) as img:
        width, height = img.size
        channels = len(img.getbands())
        
        # Get directory and base filename without extension
        dir_path  = os.path.dirname(image_path)
        base_name = os.path.splitext(os.path.basename(image_path))[0]
        
        # Create output filename with dimensions
        output_filename = f"{base_name}_{width}x{height}x{channels}.bin"
        return os.path.join(dir_path, output_filename)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Extract image pixels and save to binary file")
    parser.add_argument("image_path", help="Path to the input image file")
    parser.add_argument("-o", "--output", help="Output binary file path (default: <image_name>_<width>x<height>x<channels>.bin)")
    
    args = parser.parse_args()
    
    image_path  = args.image_path
    binary_path = args.output if args.output else generate_output_filename(image_path)

    extract_and_save_pixels_to_binary(image_path, binary_path)
    print(f"the image pixels saved to '{binary_path}'")