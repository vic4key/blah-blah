import os, sys
import numpy as np

def rgb_to_nv21(rgb_bytes, width, height, output_filename):
    """
    Converts RGB bytes to NV21 (YUV420SP) format and saves to a file.

    Args:
    rgb_bytes: Bytes representing the RGB image data (BGR order).
    width:  Width of the image in pixels.
    height: Height of the image in pixels.
    output_filename: The filename to save the NV21 data to.
    """

    # Convert RGB bytes to a NumPy array
    rgb_array = np.frombuffer(rgb_bytes, dtype=np.uint8).reshape(height, width, 3)

    # Convert RGB to YCbCr (BGR to YCbCr)
    ycbcr_array = np.zeros((height, width, 3), dtype=np.uint8)
    ycbcr_array[:, :, 0] = 0.114 * rgb_array[:, :, 0] + 0.587 * rgb_array[:, :, 1] + 0.299 * rgb_array[:, :, 2]  # Y
    ycbcr_array[:, :, 1] = -0.321264 * rgb_array[:, :, 0] - 0.704556 * rgb_array[:, :, 1] + 0.5 * rgb_array[:, :, 2] + 128  # Cb
    ycbcr_array[:, :, 2] = 0.5 * rgb_array[:, :, 0] - 0.418688 * rgb_array[:, :, 1] - 0.081312 * rgb_array[:, :, 2] + 128  # Cr

    # Downsample Cb and Cr to half resolution
    cb_cr_array = ycbcr_array[:, :, 1:].reshape(height, width, 2)
    cb_cr_array = cb_cr_array[::2, ::2, :]

    # Concatenate Y and CbCr arrays in NV21 format
    y_data = ycbcr_array[:, :, 0].astype(np.uint8).tobytes()
    cb_cr_data = cb_cr_array.astype(np.uint8).tobytes()
    nv21_data = y_data + cb_cr_data

    # Save the NV21 data to the specified file
    with open(output_filename, 'wb') as f:
        f.write(nv21_data)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python conv_rgb_to_nv21.py <path/to/rgb_data_path> <width> <height>")
        sys.exit(0)

    rgb_data_path = sys.argv[1]
    width  = int(sys.argv[2])
    height = int(sys.argv[3])

    with open(rgb_data_path, "rb") as f:
        rgb_data = f.read()

    nv21_data_path = os.path.splitext(rgb_data_path)[0] + ".nv21.bin"

    rgb_to_nv21(rgb_data, width, height, nv21_data_path)
    print(f"the nv21 data saved to '{nv21_data_path}'")
