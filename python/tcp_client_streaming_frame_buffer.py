# pip install --break-system-packages numpy opencv-python
# python3 tcp_client_streaming_frame_buffer.py

import asyncio, struct, cv2, time
from datetime import datetime
import numpy as np

def get_current_date_time():
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]

def tprint(*args, **kwargs):
    ''' Print message with current date time
    '''
    date_time = get_current_date_time()
    message = f"{date_time} [A] " + " ".join(map(str, args))
    print(message, **kwargs)

def format_bytes(num, unit=('B', 1000)): # ('iB', 1024)
    unit_prefix, unit_size = unit
    for unit in ['', 'K', 'M', 'G', 'T', 'P', 'E', 'Z']:
        if abs(num) < float(unit_size):
            return f"{num:3.1f} {unit}{unit_prefix}"
        num /= float(unit_size)
    return f"{num:.2f} Yi{unit_prefix}"

def profiling(fn):
  def _fn_wrapper(*args, **kwargs):
    start_time = time.perf_counter()
    result = fn(*args, **kwargs)
    delta_time = time.perf_counter() - start_time
    tprint("%s(...) -> %.3fs" % (fn.__qualname__, delta_time))
    return result
  return _fn_wrapper

WINDOW_NAME = "CIP20 - Virtual Display"
IDXGIImage_MAGIC = 0x24242424

def IDXGIImage_parse_image_header(image_header_bytes):
    header = struct.unpack('<5I', image_header_bytes)
    width = header[0]
    height = header[1]
    channels = header[2]
    pixel_size = header[3]
    pixel_format = header[4]
    return (width, height, channels, pixel_size, pixel_format)

def display_frame_buffer(image_buffer, width, height, channels):
    # convert the flat frame buffer to a numpy array
    pixel_data_bgra_temp = np.frombuffer(image_buffer, dtype=np.uint8)
    
    # reshape the array to (height, width, channels) as BGRA format
    pixel_data_bgra = pixel_data_bgra_temp.reshape((height, width, channels))
    
    # display the frame
    cv2.namedWindow(WINDOW_NAME, cv2.WINDOW_FULLSCREEN)
    cv2.setWindowProperty(WINDOW_NAME, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    cv2.imshow(WINDOW_NAME, pixel_data_bgra)

async def tcp_recv_packet(tcp_reader):
    __beg_time = time.time()
    __total_received_bytes = 0
    while True:
        # check for key press to quit
        key = cv2.waitKey(1) & 0xFF
        if key == 27: # ESC key
            return False

        # read image header from the socket
        image_header_bytes = await tcp_reader.readexactly(20) # 20 bytes of header
        __total_received_bytes += len(image_header_bytes)
        magic, width, height, channels, _ = IDXGIImage_parse_image_header(image_header_bytes)
        if magic == IDXGIImage_MAGIC:
            # read pixel data from the socket and display the frame
            image_buffer = await tcp_reader.readexactly(width * height * channels)
            display_frame_buffer(image_buffer, width, height, channels)
            __total_received_bytes += len(image_buffer)

        # calculate elapsed time
        __end_time = time.time()
        __delta_time = __end_time - __beg_time
        if __delta_time >= 1.0:
            __bytes_per_second = __total_received_bytes / __delta_time
            __beg_time = __end_time
            __total_received_bytes = 0
            __date_time = get_current_date_time()
            tprint(f"\r{' '*80}\r{__date_time} [A] Bandwidth = {format_bytes(__bytes_per_second)}/s", end='')

async def main(host, port):
    # connect to the server
    tcp_reader, tcp_writer = await asyncio.open_connection(host, port)
    tprint(f"TCP Client connected to the server at {host}:{port}.")

    try:
        # start receiving packet
        await tcp_recv_packet(tcp_reader)
    except Exception as e:
        tprint("TCP Client disconnected from the server.", e)
    finally:
        tcp_writer.close()
        await tcp_writer.wait_closed()
        cv2.destroyAllWindows()

try:
    # run the main function as async
    server = ("127.0.0.1", 1609)
    asyncio.run(main(*server))
except Exception as e:
    tprint(f"The TCP client could not connect to the server. The server is not running.")
except KeyboardInterrupt: tprint("User cancelled.")