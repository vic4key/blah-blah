import os, sys
import numpy as np

def rgba_to_yuv420sp(rgba_data, width, height, output_filename):
    """
    Converts RGBA data to YUV420SP (NV21) format and saves to a file.

    Args:
    rgba_data: Bytes representing the RGBA image data.
    width: Width of the image in pixels.
    height: Height of the image in pixels.
    output_filename: The filename to save the YUV420SP data to.
    """

    # Convert RGBA bytes to a NumPy array
    rgba_array = np.frombuffer(rgba_data, dtype=np.uint8).reshape(height, width, 4)

    # Convert RGBA to YCbCr
    ycbcr_array = np.zeros((height, width, 3), dtype=np.uint8)
    ycbcr_array[:, :, 0] = 0.299 * rgba_array[:, :, 0] + 0.587 * rgba_array[:, :, 1] + 0.114 * rgba_array[:, :, 2]  # Y
    ycbcr_array[:, :, 1] = -0.168736 * rgba_array[:, :, 0] - 0.331264 * rgba_array[:, :, 1] + 0.5 * rgba_array[:, :, 2] + 128  # Cb
    ycbcr_array[:, :, 2] = 0.5 * rgba_array[:, :, 0] - 0.418688 * rgba_array[:, :, 1] - 0.081312 * rgba_array[:, :, 2] + 128  # Cr

    # Downsample Cb and Cr to half resolution
    cb_cr_array = ycbcr_array[:, :, 1:].reshape(height, width, 2)
    cb_cr_array = cb_cr_array[::2, ::2, :]

    # Concatenate Y and CbCr arrays in YUV420SP (NV21) format
    y_data = ycbcr_array[:, :, 0].astype(np.uint8).tobytes()
    cb_cr_data = cb_cr_array.astype(np.uint8).tobytes()
    yuv420sp_data = y_data + cb_cr_data

    # Save the YUV420SP data to the specified file
    with open(output_filename, 'wb') as f:
        f.write(yuv420sp_data)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python conv_rgba_to_yuv420sp.py <path/to/rgba_data_path> <width> <height>")
        sys.exit(0)

    rgba_data_path = sys.argv[1]
    width  = int(sys.argv[2])
    height = int(sys.argv[3])

    with open(rgba_data_path, "rb") as f:
        rgba_data = f.read()

    yuv420sp_data_path = os.path.splitext(rgba_data_path)[0] + ".yuv420sp.bin"

    rgba_to_yuv420sp(rgba_data, width, height, yuv420sp_data_path)
    print(f"the yuv420sp data saved to '{yuv420sp_data_path}'", len(rgba_data))
