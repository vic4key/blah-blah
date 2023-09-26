import requests, time, random

response = requests.get('https://raw.githubusercontent.com/vic4key/Vutils/master/include/Vutils.h', verify=False)
assert response.status_code == 200

for line in response.iter_lines(decode_unicode=True, delimiter=None):
    print(line)
    time.sleep(random.randrange(0, 5) / 10)

# for chunk in response.iter_content(chunk_size=100, decode_unicode=True):
#     print(chunk)
#     time.sleep(random.randrange(0, 5) / 10)
