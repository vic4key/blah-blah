import os, sys

if len(sys.argv) < 2:
    print(r"Usage: download_file.py <link file> [<file name>] [<chunk size>]")
    sys.exit(1)

from urllib.parse import unquote

link_file  = sys.argv[1]
file_name  = sys.argv[2] if len(sys.argv) == 3 else unquote(link_file).split("/")[-1]
chunk_size = sys.argv[3] if len(sys.argv) == 4 else 1024

print(f"Link File  = '{link_file}'")
print(f"File Name  = '{file_name}'")
print(f"Chunk Size = {chunk_size} bytes")

try:
    import requests
    from tqdm import tqdm
    from urllib3 import disable_warnings
    disable_warnings()

    path = "."
    if not os.path.exists(path): os.makedirs(path)
    path = path.replace('\\','/') # needed for Windows
    path = os.path.join(path, file_name)

    with open(path, "wb") as f:
        r = requests.get(link_file, stream=True, verify=False)
        file_size = int(r.headers["content-length"])
        with tqdm(ncols=100, desc=f"Fetching '{file_name}'", total=file_size, unit_scale=True) as pbar:
            # 1k for chunk_size, since Ethernet packet size is around 1500 bytes
            for chunk in r.iter_content(chunk_size=chunk_size):
                f.write(chunk)
                pbar.update(chunk_size)
    print("Completed")

except:
    if os.path.exists(path): os.remove(path)
    print("Failed")
